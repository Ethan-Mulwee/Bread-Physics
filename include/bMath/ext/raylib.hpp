#ifndef BMATH_RAYLIB
#define BMATH_RAYLIB

#include <raylib.h>
#include "../vector.hpp"
#include "../matrix.hpp"

Vector3 toRay(const bMath::Vector<float,3> &v) {
  return Vector3{v.x,v.y,v.z};
}

Matrix toRay(const bMath::Matrix4 &m) {
  return Matrix{
    m(0,0), m(0,1), m(0,2), m(0,3),
    m(1,0), m(1,1), m(1,2), m(1,3),
    m(2,0), m(2,1), m(2,2), m(2,3),
    m(3,0), m(3,1), m(3,2), m(3,3)
  };
}

bMath::float3 toBread(const Vector3 &v) {
  return bMath::float3(v.x,v.y,v.z);
}

bMath::Matrix4 toBread(const Matrix &m) {
  return bMath::Matrix4(
    m.m0, m.m4, m.m8, m.m12,
    m.m1, m.m5, m.m9, m.m13,
    m.m2, m.m6, m.m10, m.m14,
    m.m3, m.m7, m.m11, m.m15
  );
}

#endif