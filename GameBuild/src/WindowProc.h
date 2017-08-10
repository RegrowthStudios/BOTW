#pragma once

// We can add custom accelerator tables here for multi-key combinations (Ctrl+C for copy, Ctrl+V for paste, etc.)
// TODO: IDs and accel structs declared here.

// This is the main procedure that is called when the operating system receives an event.
// It should properly handle most events.
LRESULT CALLBACK gameWindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Properly obtain and dispatch system events.
bool performMessageLoop(HWND hWnd);
