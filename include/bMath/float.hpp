#ifndef BMATH_FLOAT
#define BMATH_FLOAT

#include <math.h>

namespace bMath {
    inline float lerp(float a, float b, float t) {
        return a + (b-a) * t;
    }
}

#endif