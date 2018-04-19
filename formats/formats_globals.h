#ifndef FORMATS_GLOBALS_H
#define FORMATS_GLOBALS_H

#ifdef _WIN32

// This is only relevant for Windows. Though commons.dll shouldn't depend by Qt, it is harmless because on Windows
// we do require user to provide qt.
#include <QtCore/qglobal.h>

#if defined(FORMATS_LIBRARY)
#  define FORMATSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FORMATSSHARED_EXPORT Q_DECL_IMPORT
#endif

#else

#define FORMATSSHARED_EXPORT
#endif

#endif // FORMATS_GLOBALS_H
