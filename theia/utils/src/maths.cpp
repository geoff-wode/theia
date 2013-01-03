#include <utils\maths.h>

// See http://graphics.stanford.edu/~seander/bithacks.html#IntegerLogLookup.
uint32_t theia::Log2(uint32_t value)
{
  static const char LogTable256[256] = 
  {
    #define LT(n) n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n
      -1, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
      LT(4), LT(5), LT(5), LT(6), LT(6), LT(6), LT(6),
      LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7)
  };
  uint32_t r;   // r will be lg(value)
  uint32_t tt;  // temporaries

  if (tt = value >> 24) 
  {
    r = 24 + LogTable256[tt];
  } 
  else if (tt = value >> 16) 
  {
    r = 16 + LogTable256[tt];
  } 
  else if (tt = value >> 8) 
  {
    r = 8 + LogTable256[tt];
  } 
  else 
  {
    r = LogTable256[value];
  }
  return r;
}
