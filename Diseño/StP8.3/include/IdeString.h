// -*- c++ -*-

// Contains IdeString and IdeStringTokenizer class definitions

#ifndef IdeString_H
#define IdeString_H

#include "IdePlatform.h"
#include "IdeTChar.h"
#include "IdeInterlocked.h"
#include "IdeToolsApi.h"

#include <stdarg.h>
#include <iostream.h>

#if defined(_UNICODE)
#define IdeTString IdeWString
#define IdeTStringTokenizer IdeWStringTokenizer
#else // !defined(_UNICODE)
#define IdeTString IdeString
#define IdeTStringTokenizer IdeStringTokenizer
#endif // !defined(_UNICODE)


// Regular character strings
#include "IdeStringDef.h"

// Unicode strings
#define IMPLEMENT_UNICODE
#include "IdeStringDef.h"
#undef  IMPLEMENT_UNICODE

#endif // IdeString_H
