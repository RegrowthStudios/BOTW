#include "stdafx.h"
#include <timeapi.h>

#include "GameWindow.h"
#include "GameProcedures.h"
#include "RenderSystem.h" 
#include "SystemPrimitives.h"
#include "GlobalSystems.h"

extern GameProcedures g_gameProcedures{};
extern RenderSystem   g_renderSystem{};
extern ShaderManager  g_shaderManager{};

/*

The game's multithreaded architecture is organized into the following main phases:

-=- Primary Initialization
    Thread.Main
        Create all the main signaling events
        >> Spawn Thread.GraphicsInit
        Load all user configuration options
        Create the window
        Create the DX system
        >> Signal Event.DX

    Thread.GraphicsInit
        >> Wait Event.DX
        Load all primary shaders into the shader manager
        Create all default assets
        >> Signal Event.PrimaryInit

    Thread.Render
        >> Wait Event.PrimaryInit

-=- Main Loop

*/

// The entry point to the game
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    g_gameProcedures.start();

    // Create a window
    GameWindow window{};
    window.create(hInstance, nCmdShow, GameProcedures::gameWindowProcedure);
    timeBeginPeriod(1);

    // Initialize the rendering engine
    HRESULT hr = g_renderSystem.create(window);
    if (FAILED(hr)) {
        return -1;
    }
    g_gameProcedures.m_eventDXCreated.signal();

    // The message translation and dispatch loop will happen in the main thread. Ultimately 
    bool success = g_gameProcedures.performMessageLoop(window);

    // Destruction of resources
    g_gameProcedures.destroy();
    g_shaderManager.dispose();

    return success ? 0 : -1;
}
