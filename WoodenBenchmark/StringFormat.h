#pragma once

#include <string>
#include <memory>

#ifdef _MSC_VER
#define MINIGINE_FORMAT_PRINTF _snprintf
#else
#define MINIGINE_FORMAT_PRINTF snprintf
#endif

#define str_format(format, ...) \
/*Capture everything by reference*/[&] /*No arguments*/() \
{ \
    std::size_t size = MINIGINE_FORMAT_PRINTF(nullptr, 0, format, __VA_ARGS__) + 1; \
    std::unique_ptr<char[]> buf( new char[ size ] ); \
    MINIGINE_FORMAT_PRINTF(buf.get(), size, format, __VA_ARGS__); \
    return std::string(buf.get(), buf.get() + size - 1); \
} /*Call the lambda*/()
