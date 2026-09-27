#pragma once

#ifdef _WIN32
#include <windows.h>

inline void enableANSI()
{
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

  if (hOut == INVALID_HANDLE_VALUE)
    return;

  DWORD mode = 0;
  GetConsoleMode(hOut, &mode);

  mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  SetConsoleMode(hOut, mode);
}
#else
inline void enableANSI() {}
#endif

