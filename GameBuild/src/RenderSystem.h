#pragma once

class GameWindow;

struct RenderSystemArgs {
public:
    ui32v2 size;
};

class RenderSystem {
public:
    static void loadSystemConfig();

    RenderSystem();

    HRESULT create(GameWindow& window);

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

    void onWindowChange(const GameWindow& window);

    // Call this at the beginning and end of the frame to make sure that changes
    // to the swapchain/backbuffer are committed correctly.
    void onFrameStart();
    void onFrameEnd();

    // Called at the beginning of a frame when the viewport experiences a change.
    StdEvent<> m_onViewportChange;
private:
    HRESULT initResources(GameWindow& window);

    IDXGIAdapter*   m_pAdapter;
    IDXGISwapChain* m_pSwapChain;
    ID3D11Device*   m_pDevice;

    ID3D11DeviceContext*    m_pImmContext;
    ID3D11RenderTargetView* m_pBackBuffer;

    volatile bool           m_windowChangeDetected;
    D3D11_VIEWPORT          m_windowViewport;

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
