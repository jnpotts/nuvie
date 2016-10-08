#include <stdio.h>

// In Visual Studio 2015, the iob_func function no longer exists.  Recreate
// it here for the precompiled SDL libraries.
#if _MSC_VER >= 1900
FILE __iob_array[3] = { *stdin, *stdout, *stderr };
extern "C" {
	FILE* _imp____iob_func()
	{
		return __iob_array;
	}
	FILE* __imp___iob_func()
	{
		return __iob_array;
	}
}
#endif
