#ifndef ZXINGCPP_GLOBAL_H
#define ZXINGCPP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ZXINGCPP_LIBRARY)
#  define ZXINGCPP_EXPORT Q_DECL_EXPORT
#else
#  define ZXINGCPP_EXPORT Q_DECL_IMPORT
#endif

#endif // ZXINGCPP_GLOBAL_H
