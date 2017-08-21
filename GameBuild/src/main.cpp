#include "stdafx.h"

#include "GameWindow.h"
#include "GameProcedures.h"
#include "WindowProc.h"
#include "RenderSystem.h"

extern HANDLE g_hProcessTermination = INVALID_HANDLE_VALUE;

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

    // This event controls when the application is told to terminate.
    if ((g_hProcessTermination = CreateEvent(nullptr, TRUE, FALSE, L"EventTermination")) == INVALID_HANDLE_VALUE) {
        return -1;
    }

    // Create all the threads
    g_hThreadRender = CreateThread(nullptr, 0, renderProcedure, nullptr, CREATE_SUSPENDED, &g_idThreadRender);

    // Start all the threads
    ResumeThread(g_hThreadRender);

    // The message translation and dispatch loop will happen in the main thread. Ultimately 
    bool success = performMessageLoop(window.getHandle());

    { // Terminate all created threads
        // Signal and give them some time to terminate
        SetEvent(g_hProcessTermination);

        constexpr DWORD waitTimeoutMs = 300;
        DWORD result;

        HANDLE hThreads[] = {
            g_hThreadRender
        };
        constexpr size_t numThreads = sizeof(hThreads) / sizeof(hThreads[0]);
        static_assert(numThreads <= MAXIMUM_WAIT_OBJECTS, "Maximum WaitForMultipleObjects exceeded");

        // Try to wait for all the threads at once
        if (!Utils::isInRange(result = WaitForMultipleObjects(numThreads, hThreads, TRUE, waitTimeoutMs), WAIT_OBJECT_0, WAIT_OBJECT_0 + numThreads)) {
            // Forcefully terminate all threads that 
            for (size_t i = 0; i < numThreads; i++) {
                if ((result = WaitForSingleObject(hThreads[i], 0)) != WAIT_OBJECT_0) {
                    Utils::logWarning("Forcefully terminating a thread\n");
                    TerminateThread(hThreads[i], -1);
                }
            }
        }
    }

    return success ? 0 : -1;
}
