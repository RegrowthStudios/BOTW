#include "stdafx.h"
#include "WindowProc.h"
#include "GameProcedures.h"

LRESULT CALLBACK gameWindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    // TODO: Implement a dispatch system.

    // For now, we use the default window procedure for unrecognized events
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

// Typical Windows message loop implementation.
bool performMessageLoop(HWND hWnd) {
    MSG msg;
    BOOL result;
    while ((result = GetMessage(&msg, hWnd, 0, 0)) != 0) {
        if (result == -1) {
            // We encountered an error, most likely due to invalid window handle
            return false;
        }
        else {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    };
    return result != 0;
}
