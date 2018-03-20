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

    ui32 width() const;
    ui32 height() const;

    HWND getHandle() const {
        return m_hWnd;
    }
    HINSTANCE getAppInstance() const {
        return m_hInstance;
    }
private:
    HWND m_hWnd;
    HINSTANCE m_hInstance;
};
