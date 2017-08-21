#pragma once

class SysThreadContext {
public:
    friend class SysThread;

    HANDLE         m_handle;
    DWORD          m_id;

    bool shouldExit() const;
private:
    HANDLE         m_eventTerminate;

    Delegate<DWORD, const SysThreadContext&> m_fRun;
    Delegate<void>                           m_fSetToTerminate;
};

// This class is not at all thread-safe. Beware terminating threads from multiple threads.
class SysThread {
public:
    friend class SysThreadContext;

    static constexpr int WAIT_INITIAL_TERMINATE_MS = 50;
    static constexpr int WAIT_FINAL_TERMINATE_MS = 80;
    static constexpr int WAIT_ALL_INITIAL_TERMINATE_MS = 50;
    static constexpr int WAIT_ALL_FINAL_TERMINATE_MS = 80;

    static SysThread create(bool paused, Delegate<DWORD, const SysThreadContext&>&& fRun, Delegate<void>&& fSetToTerminate);
    static void      terminateAll(const SysThread** pThreads, size_t numThreads);

    SysThread(const std::shared_ptr<SysThreadContext>& pThreadCtx);

    void resume() const;
    void terminate() const;
private:
    static DWORD WINAPI runThread(LPVOID pParam);

    std::shared_ptr<SysThreadContext> m_pThreadCtx;
};


