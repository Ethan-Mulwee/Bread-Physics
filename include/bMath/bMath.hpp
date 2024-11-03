#ifndef BMATH_BMATH
#define BMATH_BMATH

#include "Vector.hpp"
#include "Matrix.hpp"
#include "Geometry.hpp"
#include <math.h>

namespace bMath {
    // Vector3 VectorRotate(const Vector3 &v, const Quaternion &q) {
    //     float x = v.x*(q.x*q.x-q.y*q.y-q.z*q.z+q.w*q.w)+v.y*(2*q.x*q.y-2*q.w*q.z)+v.z*(2*q.x*q.z+2*q.w*q.y);
    //     float y = v.x*(2*q.w*q.z+2*q.x*q.y)+v.y*(q.w*q.w-q.x*q.x+q.y*q.y-q.z*q.z)+v.z*(2*q.y*q.z-2*q.w*q.x);
    //     float z = v.x*(2*q.x*q.z-2*q.z*q.y)+v.y*(2*q.w*q.x+2*q.y*q.z)+v.z*(q.w*q.w-q.x*q.x-q.y*q.y+q.z*q.z);
    //     return Vector3(x,y,z);
    // }

    // Matrix<3,3> QuaternionToMatrix(const Quaternion &q) {
    //     Matrix<3,3> m;
    //     // i hat
    //     m.data[0][0] = q.x*q.x-q.y*q.y-q.z*q.z+q.w*q.w;
    //     m.data[1][0] = 2*q.w*q.z+2*q.x*q.y;
    //     m.data[2][0] = 2*q.x*q.z-2*q.w*q.y;
    //     // j hat
    //     m.data[0][1] = 2*q.x*q.y-2*q.w*q.z;
    //     m.data[1][1] = q.w*q.w-q.x*q.x+q.y*q.y-q.z*q.z;
    //     m.data[2][1] = 2*q.w*q.x+2*q.y*q.z;
    //     // k hat
    //     m.data[0][2] = 2*q.x*q.z+2*q.w*q.y;
    //     m.data[1][2] = 2*q.y*q.z-2*q.w*q.x;
    //     m.data[2][2] = q.w*q.w-q.x*q.x-q.y*q.y+q.z*q.z;

    //     return m;
    // }


}

#endif
