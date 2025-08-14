#ifndef DATATYPES_GLOBAL_H
#define DATATYPES_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DATATYPES_LIBRARY)
#define DATATYPES_EXPORT Q_DECL_EXPORT
#else
#define DATATYPES_EXPORT Q_DECL_IMPORT
#endif

#endif // DATATYPES_GLOBAL_H
