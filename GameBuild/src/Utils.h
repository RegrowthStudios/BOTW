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
}
