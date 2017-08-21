#include "stdafx.h"
#include "GameProcedures.h"

extern HANDLE g_hThreadRender = INVALID_HANDLE_VALUE;
extern DWORD  g_idThreadRender = 0;

// This thread is responsible for presenting the graphics.
DWORD WINAPI renderProcedure(LPVOID lpParameter) {
    while (WaitForSingleObject(g_hProcessTermination, 0) != WAIT_OBJECT_0) {
        Sleep(16);
        Utils::logWarning("Faking present\n");
    }

    return 0;
}
