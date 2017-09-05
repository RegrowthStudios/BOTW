#pragma once

class SysThreadContext {
public:
    friend class SysThread;

    HANDLE         m_handle;
    DWORD          m_id;
    void          *m_pArg;

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

    static SysThread create(bool paused, Delegate<DWORD, const SysThreadContext&>&& fRun, Delegate<void>&& fSetToTerminate, void *pArg);
    static void      terminateAll(const SysThread** pThreads, size_t numThreads);

    SysThread();
    SysThread(const std::shared_ptr<SysThreadContext>& pThreadCtx);

    void resume() const;
    void terminate() const;
private:
    static DWORD WINAPI runThread(LPVOID pParam);

    std::shared_ptr<SysThreadContext> m_pThreadCtx;
};


class SysEvent {
public:
    SysEvent() : m_hEvent(nullptr) {
        // Empty
    }
    SysEvent(const wchar_t* name) {
        create(name);
    }

    virtual ~SysEvent() {
        destroy();
    }

    SysEvent(const SysEvent&) = delete;
    SysEvent& operator=(const SysEvent&) = delete;
    
    SysEvent(SysEvent&& other) :
        m_hEvent(other.m_hEvent) {
        other.m_hEvent = nullptr;
    }
    SysEvent& operator=(SysEvent&& other) {
        destroy();
        m_hEvent = other.m_hEvent;
        other.m_hEvent = nullptr;
        return *this;
    }

    enum class Status {
        EVENT_TRUE,
        EVENT_TIMEOUT,
        EVENT_ERROR
    };

    bool reset() {
        return ResetEvent(m_hEvent) != 0;
    }

    bool signal() {
        return SetEvent(m_hEvent) != 0;
    }

    Status wait(ui32 milliseconds) const {
        DWORD result = WaitForSingleObject(m_hEvent, milliseconds);
        switch (result) {
            case WAIT_OBJECT_0:
                return Status::EVENT_TRUE;
            case WAIT_TIMEOUT:
                return Status::EVENT_TIMEOUT;
            default:
                return Status::EVENT_ERROR;
        }
    }

    Status wait() const {
        return wait(INFINITE);
    }

    operator bool() const {
        return WaitForSingleObject(m_hEvent, 0) == WAIT_OBJECT_0;
    }
private:
    void create(const wchar_t* name) {
        m_hEvent = CreateEvent(nullptr, TRUE, FALSE, name);
    }
    void destroy() {
        if (m_hEvent) {
            CloseHandle(m_hEvent);
            m_hEvent = nullptr;
        }
    }

    HANDLE m_hEvent;
};



