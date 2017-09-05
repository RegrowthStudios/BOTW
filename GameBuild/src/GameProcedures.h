#pragma once
#include "SystemPrimitives.h"

class GameWindow;

class GameProcedures {
public:
    // This is the main procedure that is called when the operating system receives an event.
    // It should properly handle most events.
    static LRESULT CALLBACK gameWindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    // This thread loads and initializes most default graphics-related assets and controllers
    static DWORD graphicsInit(const SysThreadContext& pThreadCtx);

    // This thread is responsible for presenting the graphics.
    static DWORD renderProcedure(const SysThreadContext& pThreadCtx);

    GameProcedures();

    void start();
    void destroy();

    // Properly obtain and dispatch system events.
    bool performMessageLoop(const GameWindow& window);

    SysEvent m_eventDXCreated;
    SysEvent m_eventPrimaryInit;
private:
    SysThread m_threadGraphicsInit;
    SysThread m_threadRender;

    // We can add custom accelerator tables here for multi-key combinations (Ctrl+C for copy, Ctrl+V for paste, etc.)
    // TODO: IDs and accel structs declared here.
};
