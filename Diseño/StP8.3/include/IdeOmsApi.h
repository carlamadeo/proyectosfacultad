
// File: IdeOMSApi.h
// "Copyright (C) IDE 1984-1996"

#ifndef IDE_OMS_API

#if !defined(WIN32) || !defined(WIN32DLL)
#define IDE_OMS_API       extern
#define IDE_OMS_CLASS_API
#else
#if defined (__OMS_LIB)
#define IDE_OMS_API       extern __declspec(dllexport)
#define IDE_OMS_CLASS_API __declspec(dllexport)
#else
#define IDE_OMS_API       __declspec(dllimport)
#define IDE_OMS_CLASS_API __declspec(dllimport)
#endif
#endif

#endif
