#pragma once

#define DELETE_TYPE_COPY(TYPE) \
    TYPE(const TYPE &) = delete; \
    TYPE & operator=(const TYPE &) = delete

#define DELETE_TYPE_MOVE(TYPE) \
    TYPE(TYPE &&) = delete; \
    TYPE & operator=(TYPE &&) = delete

namespace Utils {
    template<typename T>
    constexpr bool isInRange(T v, T min, T max) {
        return (min <= v) && (v <= max);
    }

    template<typename T, typename From>
    T reinterpret_offset(From v, ptrdiff_t offset = 0) {
        ptrdiff_t ptr = (ptrdiff_t)v;
        return (T)(ptr + offset);
    }

    template<typename... Params>
    int logWarning(const wchar_t* format, Params... params) {
        constexpr size_t bufferSize = 2048;
        wchar_t buf[bufferSize];
        int result = swprintf_s(buf, format, params...);
        if (result != -1) {
            OutputDebugStringW(buf);
        }
        return result;
    }

    template<typename... Params>
    int logWarning(const char* format, Params... params) {
        constexpr size_t bufferSize = 2048;
        char buf[bufferSize];
        int result = sprintf_s(buf, format, params...);
        if (result != -1) {
            OutputDebugStringA(buf);
        }
        return result;
    }

    template<typename T>
    void comRelease(T*& pObject) {
        if (pObject) {
            pObject->Release();
            pObject = nullptr;
        }
    }
}

class QPCTimer {
public:
    QPCTimer() {
        QueryPerformanceFrequency(&m_frequency);
        setStart();
    }

    void setStart() {
        QueryPerformanceCounter(&m_startPoint);
    }
    f64 delta() {
        LARGE_INTEGER cur;
        QueryPerformanceCounter(&cur);
        return (f64)(cur.QuadPart - m_startPoint.QuadPart) / (f64)m_frequency.QuadPart;
    }
    f64 deltaAndSetStart() {
        LARGE_INTEGER prev = m_startPoint;
        QueryPerformanceCounter(&m_startPoint);
        return (f64)(m_startPoint.QuadPart - prev.QuadPart) / (f64)m_frequency.QuadPart;
    }
private:
    LARGE_INTEGER m_frequency;
    LARGE_INTEGER m_startPoint;
};

