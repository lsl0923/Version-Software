#ifndef IMAGELOAD_GLOBAL_H
#define IMAGELOAD_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(IMAGELOAD_LIBRARY)
#define IMAGELOAD_EXPORT Q_DECL_EXPORT
#else
#define IMAGELOAD_EXPORT Q_DECL_IMPORT
#endif

#endif // IMAGELOAD_GLOBAL_H
