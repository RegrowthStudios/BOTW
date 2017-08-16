#pragma once

struct RenderSystemArgs {
public:
    ui32v2 size;
};

class RenderSystem {
public:
    static void loadSystemConfig();

    HRESULT create(HWND hWnd);
private:
    IDXGIAdapter*   m_pAdapter;
    IDXGISwapChain* m_pSwapChain;
    ID3D11Device*   m_pDevice;

    std::vector<D3D_FEATURE_LEVEL> m_featureLevels;
    bool                           m_isRelease    : 1;
    bool                           m_isValidating : 1;
    bool                           m_isDebuggable : 1;

    struct CreateDeviceArgs {
        IDXGIAdapter1* pAdapter;
        D3D_FEATURE_LEVEL featureLevel;
    };
    std::vector<CreateDeviceArgs> m_possibleDevices;
};
