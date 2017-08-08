#pragma once

class GameWindow {
public:
    GameWindow();
    virtual ~GameWindow();

    // Only single instances allowed
    DELETE_TYPE_COPY(GameWindow);
    DELETE_TYPE_MOVE(GameWindow);

    void create(HINSTANCE hInstance, int nCmdShow, WNDPROC pWindowProcedure);
    void destroy();

    HWND getHandle() const {
        return m_hWnd;
    }
private:
    HWND m_hWnd;
};
