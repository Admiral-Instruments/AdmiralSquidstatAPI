#pragma once

#include <QtGlobal>

/**/
#ifndef BUILD_STATIC
#if defined(SQUIDSTATLIBRARY_LIB)
#define SQUIDSTATLIBRARY_EXPORT Q_DECL_EXPORT
#else
#define SQUIDSTATLIBRARY_EXPORT Q_DECL_IMPORT
#endif
#else
#define SQUIDSTATLIBRARY_EXPORT
#endif
