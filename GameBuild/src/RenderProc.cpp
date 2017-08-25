#include "stdafx.h"
#include "GameProcedures.h"

#include "RenderSystem.h"
#include "Vorb/Timing.h"

extern SysThread g_threadRender = SysThread(nullptr);


// This thread is responsible for presenting the graphics.
DWORD renderProcedure(const SysThreadContext& pThreadCtx) {
    RenderSystem& renderSystem = *reinterpret_cast<RenderSystem*>(pThreadCtx.m_pArg);
    ID3D11DeviceContext* pImmContext = renderSystem.getImmContext();

    constexpr float speed = 0.4f;
    constexpr float presentFramerate = 60.0f;
    float x = 0.0f;
    f32v4 clearColor(0.0f, 0.0f, 0.0f, 1.0f);

    QPCTimer timer;

    while (!pThreadCtx.shouldExit()) {
        renderSystem.onFrameStart();

        x = fmodf(x + speed / presentFramerate, 3.14159f);
        clearColor.r = abs(sin(x + 0.0f));
        clearColor.g = abs(sin(x + 1.0f));
        clearColor.b = abs(sin(x + 2.7f));
        pImmContext->ClearRenderTargetView(renderSystem.getBackBufferRTV(), clearColor.data);

        // Render End
        f64 renderTime = timer.deltaAndSetStart();

        // Present contents
        renderSystem.onFrameEnd();
        f64 presentTime = timer.deltaAndSetStart();

        char buf[128];
        sprintf_s(buf, "Render(%.7f)  Present(%.7f)  FPS(%.3f)\n", renderTime, presentTime, 1 / (renderTime + presentTime));
        OutputDebugStringA(buf);
    }

    return 0;
}
