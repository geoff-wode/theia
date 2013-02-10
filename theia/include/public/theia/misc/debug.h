/// Basic debugging aids.

#if ! defined(__THEIA_DEBUG__)
#define __THEIA_DEBUG__

#include <stdio.h>
#include <stdarg.h>

#define LOG(msg, ...) fprintf(stderr, msg, __VA_ARGS__)


#endif // __THEIA_DEBUG__
