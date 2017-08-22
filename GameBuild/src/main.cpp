#include "stdafx.h"

#include "GameWindow.h"
#include "GameProcedures.h"
#include "WindowProc.h"
#include "RenderSystem.h" 
#include "SystemPrimitives.h"

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

    // Create all the threads
    g_threadRender = SysThread::create(true, del::create(renderProcedure), del::createFromFunctor([]() {}), &renderSystem);

    // Start all the threads
    g_threadRender.resume();

    // The message translation and dispatch loop will happen in the main thread. Ultimately 
    bool success = performMessageLoop(window.getHandle());

    { // Terminate all created threads
        const SysThread* pThreads[] = {
            &g_threadRender
        };
        SysThread::terminateAll(pThreads, sizeof(pThreads) / sizeof(pThreads[0]));
    }

    return success ? 0 : -1;
}
