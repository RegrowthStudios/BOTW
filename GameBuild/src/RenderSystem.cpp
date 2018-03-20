#include "stdafx.h"
#include "RenderSystem.h"

#include "GameWindow.h"

void RenderSystem::loadSystemConfig() {
    // Empty
}

RenderSystem::RenderSystem() :
    m_onViewportChange(this) {
    // Empty
}

HRESULT RenderSystem::create(GameWindow& window, bool useOpenGL) {
    // For now, reset the options until we correctly load them in
    m_isRelease = false;
    m_isValidating = false;
    m_isDebuggable = true;

    return useOpenGL ? createGL(window) : createD3D(window);
}

void RenderSystem::onWindowChange(const GameWindow& window) {
    m_windowViewport.MinDepth = 0.0f;
    m_windowViewport.MaxDepth = 1.0f;
    m_windowViewport.Width = (f32)window.width();
    m_windowViewport.Height = (f32)window.height();
    m_windowViewport.TopLeftX = 0.0f;
    m_windowViewport.TopLeftY = 0.0f;
    m_windowChangeDetected = true;
}

void RenderSystem::onFrameStart() {
    if (m_windowChangeDetected) {
        m_pImmContext->RSSetViewports(1, &m_windowViewport);
        m_onViewportChange();
        m_windowChangeDetected = false;
    }
}

void RenderSystem::onFrameEnd() {
    m_pSwapChain->Present(1, 0);
}

HRESULT RenderSystem::initResources(GameWindow& window) {
    ID3D11Texture2D* pBackBufferTex;
    m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBufferTex));
    m_pDevice->CreateRenderTargetView(pBackBufferTex, NULL, &m_pBackBuffer);
    pBackBufferTex->Release();

    // This render target is the backbuffer
    m_pImmContext->OMSetRenderTargets(1, &m_pBackBuffer, NULL);
    onWindowChange(window);

    return S_OK;
}

bool RenderSystem::createGL(GameWindow& window) {
    HDC hDeviceContext;

    PIXELFORMATDESCRIPTOR pixelFormat ={ 0 };
    pixelFormat.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pixelFormat.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
    pixelFormat.iPixelType = PFD_TYPE_RGBA;
    pixelFormat.cColorBits = 32;
    pixelFormat.cDepthBits = 32;
    pixelFormat.iLayerType = PFD_MAIN_PLANE;

    {
        // Create a temporary window to get a good OpenGL context
        WNDCLASSEX tempWindowClass;
        tempWindowClass.cbSize        = sizeof(WNDCLASSEX);
        tempWindowClass.cbClsExtra    = 0;
        tempWindowClass.cbWndExtra    = 0;
        tempWindowClass.lpszClassName = L"FakeWindow";
        tempWindowClass.lpszMenuName  = L"FakeWindow.Menu";
        tempWindowClass.style         = CS_HREDRAW | CS_VREDRAW;
        tempWindowClass.hInstance     = window.getAppInstance();
        tempWindowClass.lpfnWndProc   = DefWindowProc;
        tempWindowClass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
        tempWindowClass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
        tempWindowClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
        tempWindowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
        RegisterClassEx(&tempWindowClass);

        HWND hTempWindow = CreateWindow(
            L"FakeWindow", L"Empty",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            640, 480,
            NULL, NULL,
            window.getAppInstance(),
            NULL
        );

        hDeviceContext = GetDC(hTempWindow);
        auto chosenPixelFormat = ChoosePixelFormat(hDeviceContext, &pixelFormat);
        SetPixelFormat(hDeviceContext, chosenPixelFormat, &pixelFormat);

        m_glc = wglCreateContext(hDeviceContext);
        wglMakeCurrent(hDeviceContext, m_glc);

        // Init GL API
        //glewExperimental = true;
        bool result = glewInit() == GLEW_OK;
        if (result) {
            result &= wglewIsSupported("WGL_ARB_create_context") == 1;
        }

        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(m_glc);
        ReleaseDC(hTempWindow, hDeviceContext);
        DestroyWindow(hTempWindow);

        if (!result) {
            return false;
        }
    }

    hDeviceContext = GetDC(window.getHandle());

    const int iPixelFormatAttribList[] ={
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB, 32,
        WGL_DEPTH_BITS_ARB, 24,
        WGL_STENCIL_BITS_ARB, 8,
        0
    };
    int chosenPixelFormat = 0;
    UINT numFormats = 0;
    wglChoosePixelFormatARB(hDeviceContext, iPixelFormatAttribList, NULL, 1, &chosenPixelFormat, &numFormats);
    SetPixelFormat(hDeviceContext, chosenPixelFormat, &pixelFormat);

    int attributes[] ={
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 5,
        0
    };
    m_glc = wglCreateContextAttribsARB(hDeviceContext, 0, attributes);
    wglMakeCurrent(NULL, NULL);
    wglMakeCurrent(hDeviceContext, m_glc);

    return true;
}

bool RenderSystem::createD3D(GameWindow& window) {
    m_featureLevels.clear();
    m_featureLevels.emplace_back(D3D_FEATURE_LEVEL_11_1);

    // Clear all possibilities
    for (CreateDeviceArgs& arg : m_possibleDevices) {
        arg.pAdapter->Release();
    }
    m_possibleDevices.clear();

    HRESULT hr;

    IDXGIFactory2* pFactory;
    if (FAILED(hr = CreateDXGIFactory2(0, IID_PPV_ARGS(&pFactory)))) {
        // We can't access the main rendering system???
        return false;
    }

    // Knobs to control what device we create
    D3D11_CREATE_DEVICE_FLAG createFlags;
    if (m_isRelease) {
        createFlags = D3D11_CREATE_DEVICE_PREVENT_ALTERING_LAYER_SETTINGS_FROM_REGISTRY;
    }
    else if (m_isValidating) {
        createFlags = (D3D11_CREATE_DEVICE_FLAG)(D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_DEBUGGABLE);
    }
    else if (m_isDebuggable) {
        createFlags = D3D11_CREATE_DEVICE_DEBUGGABLE;
    }
    else {
        createFlags = (D3D11_CREATE_DEVICE_FLAG)0;
    }

    CreateDeviceArgs possible;
    for (UINT i = 0; pFactory->EnumAdapters1(i, &possible.pAdapter) != DXGI_ERROR_NOT_FOUND; ++i) {
        DXGI_ADAPTER_DESC1 desc;
        if (FAILED(hr = possible.pAdapter->GetDesc1(&desc))) {
            possible.pAdapter->Release();
            continue;
        }

        ID3D11Device* pDevice;
        ID3D11DeviceContext* pContext;
        if (FAILED(hr = D3D11CreateDevice(possible.pAdapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, createFlags, m_featureLevels.data(), (UINT)m_featureLevels.size(), D3D11_SDK_VERSION, &pDevice, &possible.featureLevel, &pContext))) {
            possible.pAdapter->Release();
            continue;
        }

        m_possibleDevices.emplace_back(std::move(possible));
        possible ={ 0 };
        pDevice->Release();
        pContext->Release();
    }

    if (m_possibleDevices.size() > 0) {
        CreateDeviceArgs& arg = m_possibleDevices[0];
        m_pAdapter = arg.pAdapter;

        DXGI_SWAP_CHAIN_DESC swapChainDesc;
        ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
        swapChainDesc.BufferCount       = 1;
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferUsage       = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.OutputWindow      = window.getHandle();
        swapChainDesc.SampleDesc.Count  = 1;
        swapChainDesc.Windowed          = true;
        if (SUCCEEDED(hr = D3D11CreateDeviceAndSwapChain(arg.pAdapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, createFlags, &arg.featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &m_pSwapChain, &m_pDevice, NULL, &m_pImmContext))) {
            hr = initResources(window);
            return SUCCEEDED(hr);
        }
        else {
            // We have a problem
            return false;
        }
    }
    else {
        return false;
    }
}
