#pragma once

struct RenderSystemArgs {
public:
    ui32v2 size;
};

class RenderSystem {
public:
    static void loadSystemConfig();

    HRESULT create(HWND hWnd);

    IDXGIAdapter* getAdapter() {
        return m_pAdapter;
    }
    IDXGISwapChain* getSwapChain() {
        return m_pSwapChain;
    }
    ID3D11Device* getDevice() {
        return m_pDevice;
    }
    ID3D11DeviceContext* getImmContext() {
        return m_pImmContext;
    }
    ID3D11RenderTargetView* getBackBufferRTV() {
        return m_pBackBuffer;
    }
private:
    HRESULT initResources();

    IDXGIAdapter*   m_pAdapter;
    IDXGISwapChain* m_pSwapChain;
    ID3D11Device*   m_pDevice;

    ID3D11DeviceContext*    m_pImmContext;
    ID3D11RenderTargetView* m_pBackBuffer;

    std::vector<D3D_FEATURE_LEVEL> m_featureLevels; // What features levels we can support

    // These are options that control what features our D3D implementation supports
    bool                           m_isRelease    : 1; // No availability to debugging or validation. Deterrant to 3rd party inspectors.
    bool                           m_isValidating : 1; // D3D debug layer that validates arguments
    bool                           m_isDebuggable : 1; // Allows inspection of D3D operations (ex. by Visual Studio).

    struct CreateDeviceArgs {
        IDXGIAdapter1* pAdapter;
        D3D_FEATURE_LEVEL featureLevel;
    };
    std::vector<CreateDeviceArgs> m_possibleDevices;
};
