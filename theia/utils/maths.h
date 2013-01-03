#if ! defined(__MATHS_DEFINED__)
#define __MATHS_DEFINED__

#include <stdint.h>

namespace theia
{
  /// Return log base of value or -1 if value == 0.
  uint32_t Log2(uint32_t value);
}

#endif // __MATHS_DEFINED__
