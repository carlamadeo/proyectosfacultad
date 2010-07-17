#ifndef IdePlatform_H
#define IdePlatform_H

// Platform dependent definitions to compensate compiler deficiences

// To spy memory, please delete the new operators from the MFC code
// undefine DEBUG_NEW and define _SPY_MEM for all the files where 
// you want to spy memory
#if (defined (_WIN32) && defined(_DEBUG) && defined(_SPY_MEM))

#include <crtdbg.h>

#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

#endif

// once all compilers support the new standard we can get rid of this.
#if defined(IRIX)
#if !defined(_BOOL)
#include <stl_config.h> // ugly hack to accomodate O32 ABI - typedefs bool
#endif  // !defined(_BOOL)
#else   // !defined(IRIX)
#if !defined(_MSC_VER) || _MSC_VER < 1100 /* anything but VC++ 5.0 or higher */
#if !defined(__DECCXX) || (__DECCXX_VER < 60000000)
#if !defined(HPX11PAR)
// ECR 4435 - begin
#if (__SUNPRO_CC < 0x500)
#define bool int
#endif
// ECR 4435 - end
#define true (1==1)
#define false (!true)
#endif // !defined(HPX11PAR)
#endif // !defined(__DECCXX) || (__DECCXX_VER < 60000000)
#endif // !defined(_MSC_VER) || _MSC_VER < 1100
#endif // !defined(IRIX)

#if defined(_MSC_VER) /* VC++ only */
#pragma warning(2:4700)     // local variable used without having been initialized
#pragma warning(2:4723)     // potential divide by 0

#pragma warning(4:4250)     // inherits via dominance
#pragma warning(4:4251)     // class needs to have dll-interface to be used by clients of class
#pragma warning(4:4275)     // non dll-interface class used as base for dll-interface class
#pragma warning(4:4290)     // C++ Exception Specification ignored
#pragma warning(4:4305)     // arithmetic truncation in initialization or argument passing
#pragma warning(4:4786)     // identifier was truncated to '255' characters in the browser information
#pragma warning(4:4291)     // no matching operator delete found; memory will not be freed if initialization throws an exception
#endif // defined(_MSC_VER) /* VC++ only */

#endif // IdePlatform_H

