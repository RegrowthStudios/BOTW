#pragma once

// This is the main procedure that is called when the operating system receives an event.
// It should properly handle most events.
LRESULT CALLBACK gameWindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// This event is a signal for all threads that the application is terminated.
extern HANDLE g_hProcessTermination;

// This thread is responsible for presenting the graphics.
extern HANDLE g_hThreadRender;
extern DWORD  g_idThreadRender;
DWORD WINAPI renderProcedure(LPVOID lpParameter);

