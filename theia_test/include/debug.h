/// Basic debugging aids.

#if ! defined(__THEIA_DEBUG__)
#define __THEIA_DEBUG__

#include <assert.h>
#include <stdio.h>
#include <stdarg.h>

#define LOG(msg, ...) fprintf(stderr, msg, __VA_ARGS__)

#define ASSERT(pred) ASSERTM(pred, "")
#define ASSERTM(pred, msg, ...) \
  do \
  { \
    if (!(pred)) \
    { \
      fprintf(stderr, "%s:%d:%s - %s", #pred, msg, __VA_ARGS__); \
      assert(0); \
    } \
  } while (0)

#endif // __THEIA_DEBUG__
