#ifndef BMATH_FWD
#define BMATH_FWD

#include <math.h>

namespace bMath {
  template <typename T, std::size_t N> struct Vector;
  template<typename T> struct Quaternion;
  template <typename T, int rows, int cols> struct Matrix;
}

#endif