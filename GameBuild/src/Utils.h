#pragma once

#define DELETE_TYPE_COPY(TYPE) \
    TYPE(const TYPE &) = delete; \
    TYPE & operator=(const TYPE &) = delete

#define DELETE_TYPE_MOVE(TYPE) \
    TYPE(TYPE &&) = delete; \
    TYPE & operator=(TYPE &&) = delete