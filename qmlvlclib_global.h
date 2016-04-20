#ifndef QMLVLCLIB_GLOBAL_H
#define QMLVLCLIB_GLOBAL_H

#include <QtCore/QtGlobal>

#if defined(QMLVLCLIB_LIBRARY)
#  define QMLVLCLIB_EXPORT Q_DECL_EXPORT
#else
#  define QMLVLCLIB_EXPORT Q_DECL_IMPORT
#endif

#endif // QMLVLCLIB_GLOBAL_H

