#include "stdafx.h"

#include "GameWindow.h"
#include "WindowProc.h"
#include "RenderSystem.h"

// The entry point to the game
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Create a window
    GameWindow window{};
    window.create(hInstance, nCmdShow, gameWindowProcedure);

    RenderSystem renderSystem;
    HRESULT hr = renderSystem.create(window.getHandle());
    if (FAILED(hr)) {
        return -1;
    }

    bool success = performMessageLoop(window.getHandle());

    return success ? 0 : -1;
}
