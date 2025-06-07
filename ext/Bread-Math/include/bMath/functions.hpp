#ifndef BMATH_FUNCTIONS
#define BMATH_FUNCTIONS

#include "vector.hpp"
#include "quaternion.hpp"
#include "matrix.hpp"

namespace bMath {
  // TODO: Generalize vector transformation operations to any type not just floats

  // optimized 3x3 vector3 transformation
  inline float3 operator*(const float3 &v, const matrix3 &m) {
    return float3(
      v.x*m(0,0)+v.y*m(0,1)+v.z*m(0,2),
      v.x*m(1,0)+v.y*m(1,1)+v.z*m(1,2),
      v.x*m(2,0)+v.y*m(2,1)+v.z*m(2,2)
    );
  }

  // optimized 4x4 vector3 transformation
  inline float3 operator*(const float3 &v, const matrix4 &m) {
    return float3(
      v.x*m(0,0)+v.y*m(0,1)+v.z*m(0,2)+m(0,3),
      v.x*m(1,0)+v.y*m(1,1)+v.z*m(1,2)+m(1,3),
      v.x*m(2,0)+v.y*m(2,1)+v.z*m(2,2)+m(2,3)
    );
  }

  // optimized 4x4 vector4 transformation
  inline float4 operator*(const float4 &v, const matrix4 &m) {
    return float4(
      v.x*m(0,0)+v.y*m(0,1)+v.z*m(0,2)+v.w*m(0,3),
      v.x*m(1,0)+v.y*m(1,1)+v.z*m(1,2)+v.w*m(1,3),
      v.x*m(2,0)+v.y*m(2,1)+v.z*m(2,2)+v.w*m(2,3),
      v.x*m(3,0)+v.y*m(3,1)+v.z*m(3,2)+v.w*m(3,3)
    );
  }

    // Transform a vector by a quaternion
  template <typename T>
  Vector<T,3> rotate(const Vector<T,3> &v, const quaternion &q) {
    return Vector<T,3>(
      v.x*(q.x*q.x-q.y*q.y-q.z*q.z+q.w*q.w)+v.y*(2*q.x*q.y-2*q.w*q.z)+v.z*(2*q.x*q.z+2*q.w*q.y),
      v.x*(2*q.w*q.z+2*q.x*q.y)+v.y*(q.w*q.w-q.x*q.x+q.y*q.y-q.z*q.z)+v.z*(2*q.y*q.z-2*q.w*q.x),
      v.x*(2*q.x*q.z-2*q.w*q.y)+v.y*(2*q.w*q.x+2*q.y*q.z)+v.z*(q.w*q.w-q.x*q.x-q.y*q.y+q.z*q.z)
    );
  }


  // Transform a vector by a quaternion
  template <typename T>
  Vector<T,3> operator*(const Vector<T,3> &v, const quaternion &q) {
    return Vector<T,3>(
      v.x*(q.x*q.x-q.y*q.y-q.z*q.z+q.w*q.w)+v.y*(2*q.x*q.y-2*q.w*q.z)+v.z*(2*q.x*q.z+2*q.w*q.y),
      v.x*(2*q.w*q.z+2*q.x*q.y)+v.y*(q.w*q.w-q.x*q.x+q.y*q.y-q.z*q.z)+v.z*(2*q.y*q.z-2*q.w*q.x),
      v.x*(2*q.x*q.z-2*q.w*q.y)+v.y*(2*q.w*q.x+2*q.y*q.z)+v.z*(q.w*q.w-q.x*q.x-q.y*q.y+q.z*q.z)
    );
  }

  // TODO: make this a method or give it a better name
  // Converts a scaled axis representation of rotation stored in a vec3 using itself as the axis and it's length as it's rotation in radians for angular velocity this is it's radians per second
  template <typename T>
  Quaternion<T> vectorToQuaternion(const Vector<T,3> &v) {
    if (v.length() == 0) 
      return Quaternion<T>::identity();
    else
      return Quaternion<T>(v.length(), v);
  }

  template<typename T> matrix3 quaternionToMatrix(const Quaternion<T> &q) {
      matrix3 m;
      // i hat
      m.data[0][0] = q.x*q.x-q.y*q.y-q.z*q.z+q.w*q.w;
      m.data[1][0] = 2*q.w*q.z+2*q.x*q.y;
      m.data[2][0] = 2*q.x*q.z-2*q.w*q.y;
      // j hat
      m.data[0][1] = 2*q.x*q.y-2*q.w*q.z;
      m.data[1][1] = q.w*q.w-q.x*q.x+q.y*q.y-q.z*q.z;
      m.data[2][1] = 2*q.w*q.x+2*q.y*q.z;
      // k hat
      m.data[0][2] = 2*q.x*q.z+2*q.w*q.y;
      m.data[1][2] = 2*q.y*q.z-2*q.w*q.x;
      m.data[2][2] = q.w*q.w-q.x*q.x-q.y*q.y+q.z*q.z;

      return m;
  }

  template<typename T> 
  Quaternion<T> matrixToQuaternion(const Matrix<T,3,3> &m) {
    T qw = sqrt(1 + m(0,0) + m(1,1) + m(2,2))/(T)2;
    T qx = (m(2,1)-m(1,2))/(4*qw);
    T qy = (m(0,2)-m(2,0))/(4*qw);
    T qz = (m(1,0)-m(0,1))/(4*qw);
    return Quaternion<T>(qw,qx,qy,qz);
  } 

  template<typename T>
  Quaternion<T> basisToQuaternion(const Vector<T,3> &i, const Vector<T,3> &j, const Vector<T,3> &k) {
    T qw = sqrt(1 + i.x + j.y + k.z)/(T)2;
    T qx = (k.y-j.z)/(4*qw);
    T qy = (i.z-k.x)/(4*qw);
    T qz = (j.x-i.y)/(4*qw);
    return Quaternion<T>(qw,qx,qy,qz);
  }

  // Create a 3x3 basis matrix with the inptu vector as the x basis
  template<typename T>
  Matrix<T,3,3> basisFromVector(const Vector<T,3> &vec) {
    float3 tagentY, tagentZ;

    float3 v = normalized(vec);

    if (abs(v.y) < abs(v.z)) {
        tagentZ = float3(-v.z, 0, v.x);
        tagentY = cross(v, tagentZ);
    }
    else {
        tagentY = float3(-v.y, v.x, 0);
        tagentZ = cross(v, tagentY);
    }


    return Matrix<T,3,3>(
        v.x, tagentY.x, tagentZ.x,
        v.y, tagentY.y, tagentZ.y,
        v.z, tagentY.z, tagentZ.z
    );
  }
}

#endif