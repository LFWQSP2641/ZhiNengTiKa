#ifndef QRSCANLIB_GLOBAL_H
#define QRSCANLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QRSCANLIB_LIBRARY)
#  define QRSCANLIB_EXPORT Q_DECL_EXPORT
#else
#  define QRSCANLIB_EXPORT Q_DECL_IMPORT
#endif

#endif // QRSCANLIB_GLOBAL_H
