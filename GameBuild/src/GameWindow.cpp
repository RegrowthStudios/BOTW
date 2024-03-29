#include "stdafx.h"
#include "GameWindow.h"

GameWindow::GameWindow() :
    m_hWnd(NULL) {
    // Empty
}

GameWindow::~GameWindow() {
    destroy();
}

void GameWindow::create(HINSTANCE hInstance, int nCmdShow, WNDPROC pWindowProcedure) {
    destroy();

    m_hInstance = hInstance;

    WNDCLASSEX windowClass;
    windowClass.cbSize        = sizeof(WNDCLASSEX);
    windowClass.cbClsExtra    = 0;
    windowClass.cbWndExtra    = 0;
    windowClass.lpszClassName = CLASS_NAME_WINDOW;
    windowClass.lpszMenuName  = CLASS_NAME_MENU;
    windowClass.style         = CS_HREDRAW | CS_VREDRAW;
    windowClass.hInstance     = m_hInstance;
    windowClass.lpfnWndProc   = pWindowProcedure;
    windowClass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    windowClass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
    windowClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    RegisterClassEx(&windowClass);

    DWORD windowFlags = WS_OVERLAPPEDWINDOW;
    m_hWnd = CreateWindow(
        CLASS_NAME_WINDOW, DEFAULT_WINDOW_TITLE, windowFlags,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, m_hInstance, NULL);
    ShowWindow(m_hWnd, nCmdShow);
    UpdateWindow(m_hWnd);
}

void GameWindow::destroy() {
    if (m_hWnd) {
        DestroyWindow(m_hWnd);
        m_hWnd = nullptr;
    }
}

ui32 GameWindow::width() const {
    RECT r;
    GetClientRect(m_hWnd, &r);
    return r.right - r.left;
}

ui32 GameWindow::height() const {
    RECT r;
    GetClientRect(m_hWnd, &r);
    return r.bottom - r.top;
}
