#include "stdafx.h"
#include "SystemPrimitives.h"

bool SysThreadContext::shouldExit() const {
    return (m_handle != INVALID_HANDLE_VALUE) && (WaitForSingleObject(m_eventTerminate, 0) == WAIT_OBJECT_0);
}

DWORD WINAPI SysThread::runThread(LPVOID pParam) {
    SysThreadContext* pThreadCtx = reinterpret_cast<SysThreadContext*>(pParam);
    return pThreadCtx->m_fRun(*pThreadCtx);
}

SysThread SysThread::create(bool paused, Delegate<DWORD, const SysThreadContext&>&& fRun, Delegate<void>&& fSetToTerminate) {
    std::shared_ptr<SysThreadContext> pThreadCtx = std::make_shared<SysThreadContext>();

    pThreadCtx->m_handle = CreateThread(nullptr, 0, SysThread::runThread, pThreadCtx.get(), CREATE_SUSPENDED, &pThreadCtx->m_id);
    pThreadCtx->m_fRun = std::move(fRun);
    pThreadCtx->m_fSetToTerminate = std::move(fSetToTerminate);
    pThreadCtx->m_eventTerminate = CreateEvent(nullptr, TRUE, FALSE, L"Event.ThreadTerminate");

    if (!paused) {
        ResumeThread(pThreadCtx->m_handle);
    }

    return SysThread(pThreadCtx);
}

void SysThread::terminateAll(const SysThread** pThreads, size_t numThreads) {
    // Transfer valid threads to respective arrays
    void* pData = _alloca(numThreads * (sizeof(HANDLE) + sizeof(const SysThread*)));
    size_t c = 0;
    HANDLE* pHandles = reinterpret_cast<HANDLE*>(pData);
    const SysThread** pThreadsToTerminate = Utils::reinterpret_offset<const SysThread**>(pData, numThreads * sizeof(HANDLE));
    for (size_t i = 0; i < numThreads; i++) {
        const SysThread& thread = *pThreads[i];
        if (thread.m_pThreadCtx && (thread.m_pThreadCtx->m_handle != INVALID_HANDLE_VALUE)) {
            if (c == MAXIMUM_WAIT_OBJECTS) {
                Utils::logWarning("Can't terminate all threads simultaneously.\n");
                thread.terminate();
            }
            else {
                SetEvent(thread.m_pThreadCtx->m_eventTerminate);
                pHandles[c] = thread.m_pThreadCtx->m_handle;
                pThreadsToTerminate[c] = &thread;
                ++c;
            }
        }
    }

    if (c == 0) {
        return;
    }

    DWORD result;
    if (!Utils::isInRange(result = WaitForMultipleObjects(c, pHandles, TRUE, WAIT_ALL_INITIAL_TERMINATE_MS), WAIT_OBJECT_0, WAIT_OBJECT_0 + c - 1)) {
        // Prod threads if the initial exit event didn't work
        for (size_t i = 0; i < c; i++) {
            pThreadsToTerminate[i]->m_pThreadCtx->m_fSetToTerminate();
        }
        if (!Utils::isInRange(result = WaitForMultipleObjects(c, pHandles, TRUE, WAIT_ALL_FINAL_TERMINATE_MS), WAIT_OBJECT_0, WAIT_OBJECT_0 + c - 1)) {
            // Forcibly terminate unterminated threads
            for (size_t i = 0; i < c; i++) {
                if ((result = WaitForSingleObject(pHandles[i], 0)) != WAIT_OBJECT_0) {
                    TerminateThread(pHandles[i], -1);
                }
            }
        }
    }

    for (size_t i = 0; i < c; i++) {
        pThreadsToTerminate[i]->m_pThreadCtx->m_handle = INVALID_HANDLE_VALUE;
    }
}

SysThread::SysThread(const std::shared_ptr<SysThreadContext>& pThreadCtx) :
    m_pThreadCtx(pThreadCtx) {
    // Empty
}

void SysThread::resume() const {
    if (m_pThreadCtx && (m_pThreadCtx->m_handle != INVALID_HANDLE_VALUE)) {
        ResumeThread(m_pThreadCtx->m_handle);
    }
}

void SysThread::terminate() const {
    // If we've terminated once before, don't do it again.
    if (m_pThreadCtx && (m_pThreadCtx->m_handle != INVALID_HANDLE_VALUE)) {
        DWORD result;

        // Tell the thread that it should terminate now.
        SetEvent(m_pThreadCtx->m_eventTerminate);
        if ((result = WaitForSingleObject(m_pThreadCtx->m_handle, WAIT_INITIAL_TERMINATE_MS)) == WAIT_OBJECT_0) {
            m_pThreadCtx->m_handle = INVALID_HANDLE_VALUE;
            return;
        }

        // Prod the thread to terminate by forcing its special termination routine.
        m_pThreadCtx->m_fSetToTerminate();
        if ((result = WaitForSingleObject(m_pThreadCtx->m_handle, WAIT_FINAL_TERMINATE_MS)) == WAIT_OBJECT_0) {
            m_pThreadCtx->m_handle = INVALID_HANDLE_VALUE;
            return;
        }

        // Forcibly terminate the thread now, we've given it 2 chances.
        TerminateThread(m_pThreadCtx->m_handle, -1);
        m_pThreadCtx->m_handle = INVALID_HANDLE_VALUE;
        return;
    }
}
