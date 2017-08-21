#pragma once
#include "SystemPrimitives.h"

// This is the main procedure that is called when the operating system receives an event.
// It should properly handle most events.
LRESULT CALLBACK gameWindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// This thread is responsible for presenting the graphics.
extern SysThread g_threadRender;
DWORD renderProcedure(const SysThreadContext& pThreadCtx);
