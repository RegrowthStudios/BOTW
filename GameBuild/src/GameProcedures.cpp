#include "stdafx.h"
#include "GameProcedures.h"

#include "GlobalSystems.h"
#include "ShaderManager.h"
#include "RenderBatch.h"
#include "GameWindow.h"

LRESULT CALLBACK GameProcedures::gameWindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    // TODO: Implement a dispatch system.

    // For now, we use the default window procedure for unrecognized events
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

DWORD GameProcedures::graphicsInit(const SysThreadContext& pThreadCtx) {
    g_gameProcedures.m_eventDXCreated.wait();

    ID3D11Device* pDevice = g_renderSystem.getDevice();

    g_shaderManager.loadCompiledShaderFile(pDevice, "RenderBatch", ShaderManager::ShaderType::VERTEX, "data/shaderobj/RenderBatchVS.cso");
    g_shaderManager.loadCompiledShaderFile(pDevice, "RenderBatch", ShaderManager::ShaderType::PIXEL,  "data/shaderobj/RenderBatchPS.cso");

    g_gameProcedures.m_eventPrimaryInit.signal();

    return 0;
}

DWORD GameProcedures::renderProcedure(const SysThreadContext& pThreadCtx) {
    g_gameProcedures.m_eventPrimaryInit.wait();

    RenderSystem& renderSystem = g_renderSystem;
    ID3D11DeviceContext* pImmContext = renderSystem.getImmContext();

    constexpr float speed = 0.4f;
    constexpr float presentFramerate = 60.0f;
    float x = 0.0f;
    f32v4 clearColor(0.0f, 0.0f, 0.0f, 1.0f);

    QPCTimer timer;

    RenderBatch batch;
    batch.setShaders(g_shaderManager);
    batch.beginNewBatch(renderSystem.getDevice(), pImmContext);
    batch.draw(nullptr, f32v2(0.0f, 0.0f), f32v2(1.0f, 1.0f), ui8v4(255, 255, 0, 255));
    batch.draw(nullptr, f32v2(0.0f, 0.5f), f32v2(1.0f, 1.0f), ui8v4(255, 0, 255, 255));
    batch.endBatch();

    while (!pThreadCtx.shouldExit()) {
        renderSystem.onFrameStart();

        // Render start
        x = fmodf(x + speed / presentFramerate, 3.14159f);
        clearColor.r = abs(sin(x + 0.0f));
        clearColor.g = abs(sin(x + 1.0f));
        clearColor.b = abs(sin(x + 2.7f));
        pImmContext->ClearRenderTargetView(renderSystem.getBackBufferRTV(), clearColor.data);

        batch.renderBatch();

        // Render End
        f64 renderTime = timer.deltaAndSetStart();

        // Present contents
        renderSystem.onFrameEnd();
        f64 presentTime = timer.deltaAndSetStart();

        char buf[128];
        sprintf_s(buf, "Render(%.7f)  Present(%.7f)  FPS(%.3f)\n", renderTime, presentTime, 1 / (renderTime + presentTime));
        OutputDebugStringA(buf);
    }

    batch.dispose();

    return 0;
}

GameProcedures::GameProcedures() :
    m_eventDXCreated(),
    m_eventPrimaryInit(),
    m_threadGraphicsInit(),
    m_threadRender() {
    // Empty
}

void GameProcedures::start() {
    m_eventDXCreated           = std::move(SysEvent(L"Event.DXCreated"));
    m_eventPrimaryInit         = std::move(SysEvent(L"Event.PrimaryInit"));

    m_threadGraphicsInit       = SysThread::create(true, del::create(&GameProcedures::graphicsInit),    del::createFromFunctor([]() {}), nullptr);
    m_threadRender             = SysThread::create(true, del::create(&GameProcedures::renderProcedure), del::createFromFunctor([]() {}), nullptr);

    m_threadGraphicsInit.resume();
    m_threadRender.resume();
}

void GameProcedures::destroy() {
    m_eventDXCreated           = std::move(SysEvent());
    m_eventPrimaryInit         = std::move(SysEvent());
    
    const SysThread* threads[] = {
        &m_threadRender,
        &m_threadGraphicsInit
    };
    SysThread::terminateAll(threads, sizeof(threads) / sizeof(threads[0]));
}

bool GameProcedures::performMessageLoop(const GameWindow& window) {
    // Typical Windows message loop implementation.
    MSG msg;
    BOOL result;
    while ((result = GetMessage(&msg, window.getHandle(), 0, 0)) != 0) {
        if (result == -1) {
            // We encountered an error, most likely due to invalid window handle
            return false;
        }
        else {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    };
    return result != 0;
}