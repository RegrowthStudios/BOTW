#include "stdafx.h"
#include "GameProcedures.h"

extern SysThread g_threadRender = SysThread(nullptr);

// This thread is responsible for presenting the graphics.
DWORD renderProcedure(const SysThreadContext& pThreadCtx) {
    while (!pThreadCtx.shouldExit()) {
        Sleep(16);
        Utils::logWarning("Faking present\n");
    }

    return 0;
}
