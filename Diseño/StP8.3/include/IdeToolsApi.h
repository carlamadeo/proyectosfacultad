
// File: IdeToolsApi.h
// "Copyright (C) IDE 1984-1996"

#ifndef IDE_TOOLS_API

#if !defined(WIN32) || !defined(WIN32DLL)
#define IDE_TOOLS_API       extern
#define IDE_TOOLS_CLASS_API
#else
#if defined (__TOOLS_LIB)
#define IDE_TOOLS_API       extern __declspec(dllexport)
#define IDE_TOOLS_CLASS_API __declspec(dllexport)
#else
#define IDE_TOOLS_API       __declspec(dllimport)
#define IDE_TOOLS_CLASS_API __declspec(dllimport)
#endif
#endif

#endif
