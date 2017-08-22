#include "stdafx.h"
#include "RenderSystem.h"

void RenderSystem::loadSystemConfig() {
    // Empty
}

HRESULT RenderSystem::create(HWND hWnd) {
    // For now, reset the options until we correctly load them in
    m_isRelease = false;
    m_isValidating = false;
    m_isDebuggable = true;
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
        return hr;
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
        if (FAILED(hr = D3D11CreateDevice(possible.pAdapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, createFlags, m_featureLevels.data(), m_featureLevels.size(), D3D11_SDK_VERSION, &pDevice, &possible.featureLevel, &pContext))) {
            possible.pAdapter->Release();
            continue;
        }

        m_possibleDevices.emplace_back(std::move(possible));
        possible = { 0 };
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
        swapChainDesc.OutputWindow      = hWnd;
        swapChainDesc.SampleDesc.Count  = 1;
        swapChainDesc.Windowed          = true;
        if (SUCCEEDED(hr = D3D11CreateDeviceAndSwapChain(arg.pAdapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, createFlags, &arg.featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &m_pSwapChain, &m_pDevice, NULL, &m_pImmContext))) {
            return initResources();
        }
        else {
            // We have a problem
            return hr;
        }
    }
    else {
        return E_FAIL;
    }
}

HRESULT RenderSystem::initResources() {
    ID3D11Texture2D* pBackBufferTex;
    m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBufferTex));
    m_pDevice->CreateRenderTargetView(pBackBufferTex, NULL, &m_pBackBuffer);
    pBackBufferTex->Release();

    // This render target is the backbuffer
    m_pImmContext->OMSetRenderTargets(1, &m_pBackBuffer, NULL);
    
    return S_OK;
}
