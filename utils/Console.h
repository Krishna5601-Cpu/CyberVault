#pragma once

#ifdef _WIN32

#ifndef NOMINMAX
#define NOMINMAX
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

inline void enableANSI()
{
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

  if (hOut == INVALID_HANDLE_VALUE)
    return;

  DWORD mode = 0;

  if (!GetConsoleMode(hOut, &mode))
    return;

  SetConsoleMode(hOut,
                 mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

#else

inline void enableANSI() {}

#endif