#pragma once

// We can add custom accelerator tables here for multi-key combinations (Ctrl+C for copy, Ctrl+V for paste, etc.)
// TODO: IDs and accel structs declared here.

// Properly obtain and dispatch system events.
bool performMessageLoop(HWND hWnd);
