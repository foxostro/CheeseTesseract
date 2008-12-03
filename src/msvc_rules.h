#ifndef STDAFX_RULES_MSVC
#define STDAFX_RULES_MSVC

#if defined(_MSC_VER)
#	pragma once
#	if _MSC_VER == 1500 // 2008
#		pragma warning(disable: 4180) // qualifier applied to function type has no meaning; ignored
#	elif _MSC_VER == 1400 // 2005
#		pragma warning(disable: 4180) // qualifier applied to function type has no meaning; ignored
#		pragma warning(disable: 4312) // type cast conversion from uintptr_t to void* of greater size in xlocnum
#	elif _MSC_VER == 1310 // 2003
#		pragma warning(disable: 4702) // unreachable code in xtree and list
#		pragma warning(disable: 4512) // cannot generate assignment operator
#	elif _MSC_VER == 1300 // 2002
#		// untested
#	elif _MSC_VER < 1300 // Visual Studio 6 and below
#		pragma error("Compiler not supported!")
#	endif
#
#	// necessary for boost (_s "safe" functions are not standard yet)
#	define _CRT_NONSTDC_NO_DEPRECATE
#	define _CRT_SECURE_NO_DEPRECATE
#	define _SCL_SECURE_NO_DEPRECATE
#
#	ifdef NDEBUG // disable checked iterators in Release mode
#		ifdef _SECURE_SCL
#			undef _SECURE_SCL
#		endif
#
#		ifdef _SECURE_SCL
#			undef _HAS_ITERATOR_DEBUGGING
#		endif
#
#		define _SECURE_SCL 0
#		define _HAS_ITERATOR_DEBUGGING 0
#	endif
#endif

#endif
