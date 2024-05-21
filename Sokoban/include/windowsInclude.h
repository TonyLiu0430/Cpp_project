#pragma once
#ifdef AWG

#ifdef UNICODE
#undef UNICODE
#endif
#ifndef ASCII
#define ASCII
#endif


#ifdef _WIN32_WINNT
#ifndef _WIN32_WINNT_WIN8
#define _WIN32_WINNT_WIN8 0x0602
#endif // ifndef _WIN32_WINNT_WIN8
#if _WIN32_WINNT < _WIN32_WINNT_WIN8
#undef _WIN32_WINNT
#endif // if _WIN32_WINNT < _WIN32_WINNT_WIN8
#endif // ifdef _WIN32_WINNT

#ifndef _WIN32_WINNT
#define _WIN32_WINNT _WIN32_WINNT_WIN8
#endif

#include <windows.h>

#endif