#include "stdafx.h"

#include "GameWindow.h"

LRESULT CALLBACK gameWindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

// The entry point to the game
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Create a window
    GameWindow window{};
    window.create(hInstance, nCmdShow, gameWindowProcedure);

    {
        // Windows message loop
        MSG msg;
        BOOL result = FALSE;
        do {
            if (result) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            result = GetMessage(&msg, window.getHandle(), 0, 0);
        } while ((result != 0) && (result != -1));
    }
}
