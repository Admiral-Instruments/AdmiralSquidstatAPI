#pragma once

#include <QtGlobal>

/*
 * For the API:
 *	- We should only use the SQUIDSTATLIBRARY_EXPORT macro
 *  - Any class that should be exposed to the user should be marked with SQUIDSTATLIBRARY_EXPORT
 * For the SUI:
 *  - We should only use the SUI_SQUIDSTATLIBRARY_EXPORT macro
 *  - It will expose some internal members of the API that are shared among different libraries in the SUI
 *		- TODO: remove all the different libraries so dependencies are less complicated
 */

#ifndef BUILD_STATIC
	#if defined(SQUIDSTATLIBRARY_LIB)
		#define SQUIDSTATLIBRARY_EXPORT Q_DECL_EXPORT
		#if defined(SUI_BUILD)
			#define SUI_SQUIDSTATLIBRARY_EXPORT Q_DECL_EXPORT
		#else
			#define SUI_SQUIDSTATLIBRARY_EXPORT
		#endif
	#else
		#define SQUIDSTATLIBRARY_EXPORT Q_DECL_IMPORT
		#if defined(SUI_BUILD)
			#define SUI_SQUIDSTATLIBRARY_EXPORT Q_DECL_IMPORT
		#else
			#define SUI_SQUIDSTATLIBRARY_EXPORT
		#endif
	#endif
#else
	#define SQUIDSTATLIBRARY_EXPORT
	#define SUI_SQUIDSTATLIBRARY_EXPORT
#endif

