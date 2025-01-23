#ifndef BASIC_GLOBAL_H
#define BASIC_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(BASIC_LIBRARY)
#  define BASIC_EXPORT Q_DECL_EXPORT
#else
#  define BASIC_EXPORT Q_DECL_IMPORT
#endif

#endif // BASIC_GLOBAL_H
