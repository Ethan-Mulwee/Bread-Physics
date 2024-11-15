#ifndef BMATH_VECTOR
#define BMATH_VECTOR

#include "float.hpp"
#include "fwd.hpp"
#include <algorithm>
#include <math.h>

namespace bMath {
template <typename T, int n> struct Vector {
  T data[n];

  template <typename... Args> Vector(Args... args) : data{(T)args...} {}

  T &operator[](int i) { return data[i]; }

  T operator[](int i) const { return data[i]; }

  float magnitude() {
    float total = 0;
    for (int i = 0; i < n; i++) {
      total += data[i] * data[i];
    }
    return sqrt(total);
  }

  // TODO
  // Normalize the vector
  void normalize() {
    float mag = (*this).magnitude();
    for (int i = 0; i < n; i++) {
      data[i] = data[i] * (float)1 / mag;
    }
  }
};

template <> struct Vector<float, 2> {
  union {
    float data[2];
    struct {
      float x, y;
    };
  };

  // TODO: Add a marco to automatically create these duplicate defs
  
  // TODO: Constructor to create vectors from smaller vectors like a vec3 from a vec2

  template <typename... Args> Vector(Args... args) : data{(float)args...} {}

  float &operator[](int i) { return data[i]; }

  float operator[](int i) const { return data[i]; }

  float magnitude() const {
    float total = 0;
    for (int i = 0; i < 2; i++) {
      total += data[i] * data[i];
    }
    return sqrt(total);
  }

  // Normalize the vector
  void normalize() {
    float mag = (*this).magnitude();
    for (int i = 0; i < 2; i++) {
      data[i] = data[i] * (float)1 / mag;
    }
  }
};

// TODO: template specialization includes other types
template <> struct Vector<float, 3> {
  union {
    float data[3];
    struct {
      float x, y, z;
    };
    Vector<float, 2> xy;
  };

  template <typename... Args> Vector(Args... args) : data{(float)args...} {}

  float &operator[](int i) { return data[i]; }

  float operator[](int i) const { return data[i]; }

  float magnitude() const {
    float total = 0;
    for (int i = 0; i < 3; i++) {
      total += data[i] * data[i];
    }
    return sqrt(total);
  }

  // Normalize the vector
  void normalize() {
    float mag = (*this).magnitude();
    for (int i = 0; i < 3; i++) {
      data[i] = data[i] * (float)1 / mag;
    }
  }
};
template <> struct Vector<float, 4> {
  union {
    float data[4];
    struct {
      float x, y, z, w;
    };
  };

  template <typename... Args> Vector(Args... args) : data{(float)args...} {}

  float &operator[](int i) { return data[i]; }

  float operator[](int i) const { return data[i]; }

  float magnitude() const {
    float total = 0;
    for (int i = 0; i < 4; i++) {
      total += data[i] * data[i];
    }
    return sqrt(total);
  }

  // Normalize the vector
  void normalize() {
    float mag = (*this).magnitude();
    for (int i = 0; i < 4; i++) {
      data[i] = data[i] * (float)1 / mag;
    }
  }
};

typedef Vector<double, 2> double2;
typedef Vector<double, 3> double3;
typedef Vector<double, 4> double4;

typedef Vector<float, 2> float2;
typedef Vector<float, 3> float3;
typedef Vector<float, 4> float4;

typedef Vector<int, 2> int2;
typedef Vector<int, 3> int3;
typedef Vector<int, 4> int4;

template <typename T, int n>
Vector<T, n> operator+(const Vector<T, n> &a, const Vector<T, n> &b) {
  Vector<T, n> result;
  for (int i = 0; i < n; i++) {
    result[i] = a[i] + b[i];
  }
  return result;
}

template <typename T, int n>
void operator+=(Vector<T, n> &a, const Vector<T, n> &b) {
  for (int i = 0; i < n; i++) {
    a[i] += b[i];
  }
}

template <typename T, int n>
Vector<T, n> operator-(const Vector<T, n> &a, const Vector<T, n> &b) {
  Vector<T, n> result;
  for (int i = 0; i < n; i++) {
    result[i] = a[i] - b[i];
  }
  return result;
}

template <typename T, int n>
void operator-=(Vector<T, n> &a, const Vector<T, n> &b) {
  for (int i = 0; i < n; i++) {
    a[i] -= b[i];
  }
}

// Returns vector scaled by scalar
template <typename T, int n>
Vector<T, n> operator*(const Vector<T, n> &a, const float b) {
  Vector<T, n> result;
  for (int i = 0; i < n; i++) {
    result[i] = a[i] * b;
  }
  return result;
}

// Returns vector multipled component wise (hadamard product)
template <typename T, int n>
Vector<T, n> operator*(const Vector<T,n> &a, const Vector<T,n> &b) {
  Vector<T,n> result;
  for (int i = 0; i < n; i++) {
    result[i] = a[i]*b[i];
  }
}

// Multiples vector component wise by another vector
template <typename T, int n>
Vector<T, n> operator*=(Vector<T,n> &a, const Vector<T,n> &b) {
  for (int i = 0; i < n; i++) {
    a[i] *= b[i];
  }
}

// Transforms vector by a matrix (assuming column vector)
template <typename T, int n, int rows>
Vector<T, rows> operator*(const Vector<T,n> &v, const Matrix<T,rows,n> &m) {
  Vector<T, rows> result;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < n; j++) {
      result[i] += v[j]*m(i,j);
    }
  }
  return result;
}

// TODO: make this less slow?
// Transforms vector by a matrix bigger than the source vector (just fills 1s) (assuming column vector)
template <typename T, int n, int cols, int rows>
Vector<T, n> operator*(const Vector<T,n> &v, const Matrix<T,rows,cols> &m) {
  Vector<T, n> result;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (i >= n) return result;
      T cord;
      if (j < n) cord = v[j];
      else cord = 1;
      result[i] += cord*m(i,j);
    }
  }
  return result;
}

// Returns vector transformed by matrix (will shrink dimension of vector of transformation does so)
// template <typename T, int n, int cols>
// Vector<T, cols> transform(const Vector<T,n> &v, const Matrix<T,n,cols> &m) {
//   Vector<T, cols> result;
// }

template <typename T, int n>
float lengthSquared(const Vector<T,n> &v) {
  float total;
  for (int i = 0; i < n; i++) {
    total += v[i]*v[i];
  }
  return total;
} 

template <typename T, int n>
float dot(const Vector<T, n> &a, const Vector<T, n> &b) {
  float result = 0;
  for (int i = 0; i < n; i++) {
    result += a[i] * b[i];
  }
  return result;
}

// TODO: general wedge product
template<typename T>
Vector<T,3> wedgeProduct(const Vector<T,3> &a, const Vector<T,3> &b) {
  return Vector<T,3>();
}

// Just does geometric product for vectors of size 3 for now
// TODO: figure out how to generalize to any dim
template <typename T>
Vector<T,4> geometricProduct(const Vector<T,3> &a, const Vector<T,3> &b) {
  return Vector<T,4>(
    a.y*b.z-a.z*b.z,
    a.z*b.x-a.x*b.z,
    a.x*b.y-a.y*b.x,
    a.x*b.x+a.y*b.y+a.z*b.z
  );
}

// TODO: test on non unit vectors
template <typename T>
Vector<T,4> rotationBetween(const Vector<T,3> &a, const Vector<T,3> &b) {
  Vector<T,3> axis = cross(a,b);
  Vector<T,4> Quaternion(
    axis.x,axis.y,axis.z, dot(a,b)+1
  );
  return normalize(Quaternion);
}

template <typename T>
Vector<T, 3> cross(const Vector<T, 3> &a, const Vector<T, 3> &b) {
  return Vector<T, 3>(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

template <typename T, int n>
Vector<T, n> max(const Vector<T, n> &a, const Vector<T, n> &b) {
  Vector<T, n> result;
  for (int i = 0; i < n; i++) {
    result[i] = std::max(a[i], b[i]);
  }
  return result;
}

template <typename T, int n>
Vector<T, n> min(const Vector<T, n> &a, const Vector<T, n> &b) {
  Vector<T, n> result;
  for (int i = 0; i < n; i++) {
    result[i] = std::min(a[i], b[i]);
  }
  return result;
}

template <typename T, int n>
Vector<T, n> lerp(const Vector<T, n> &a, const Vector<T, n> &b, const float t) {
  Vector<T, n> result;
  for (int i = 0; i < n; i++) {
    result[i] = lerp(a[i], b[i], t);
  }
  return result;
}

// TODO
// slerp between two quaternions
inline float4 slerp(const float4 &a, const float4 &b, float t) {
  return float4();
}

// TODO
// Clamp length of vector
template <typename T, int n>
Vector<T, n> clamp(const Vector<T, n> &a, float b) {
  return Vector<T,n>();
}

// TODO
// Clamp vector component wise
template <typename T, int n>
Vector<T, n> clamp(const Vector<T, n> &a, const Vector<T, n>) {
  // std::clamp()
}

template <typename T, int n>
float distance(const Vector<T, n> &a, const Vector<T, n> &b) {
  return (a - b).magnitude();
}

template <typename T, int n> Vector<T, n> normalize(const Vector<T, n> &a) {
  float mag = a.magnitude();
  Vector<T, n> result;
  for (int i = 0; i < n; i++) {
    result[i] = a[i] * (float)1 / mag;
  }
  return result;
}

// Create a vector from an angle (in radians) and an axis
inline float4 QuaternionAxisAngle(const float angle, float3 axis) {
  axis.normalize();
  return float4(std::sin(angle * 0.5) * axis.x, std::sin(angle * 0.5) * axis.y, std::sin(angle * 0.5) * axis.z, std::cos(angle * 0.5));
}

// Rotate a vector by a quaternion
inline float3 rotate(const float3 &v, const float4 &q) {
  return float3(
    v.x*(q.x*q.x-q.y*q.y-q.z*q.z+q.w*q.w)+v.y*(2*q.x*q.y-2*q.w*q.z)+v.z*(2*q.x*q.z+2*q.w*q.y),
    v.x*(2*q.w*q.z+2*q.x*q.y)+v.y*(q.w*q.w-q.x*q.x+q.y*q.y-q.z*q.z)+v.z*(2*q.y*q.z-2*q.w*q.x),
    v.x*(2*q.x*q.z-2*q.w*q.y)+v.y*(2*q.w*q.x+2*q.y*q.z)+v.z*(q.w*q.w-q.x*q.x-q.y*q.y+q.z*q.z)
  );
}

// TODO: testing
// Rotate a quaternion by a vector (result = q + (1/2)*float4(0,v.x,v.y,v.z)*q)
inline float4 rotate(const float4 &q, const float3 &v) {
  float4 result(
    q.x + (0.5) * (v.x * q.w + v.y * q.z - v.z * q.y),
    q.y + (0.5) * (v.y * q.w + v.z * q.x - v.x * q.z),
    q.z + (0.5) * (v.z * q.w + v.x * q.y - v.y * q.x),
    q.w + (0.5) * (-v.x * q.x - v.y * q.y - v.z * q.z)
  );
  result.normalize();
  return result;
}

inline float QuaternionAngle(const float4 &q) {
  return std::acos(q.w);
}

// Returns vector rotated some degrees along the x axis
inline float3 rotateX(const float3 &v, const float angle) {
  return float3(
    v.x, v.y*cos(angle)-v.z*sin(angle), v.y*sin(angle)+v.z*cos(angle)
  );
}

// Returns vector rotated some degrees along the y axis
inline float3 rotateY(const float3 &v, const float angle) {
  return float3(
    v.x*cos(angle)+v.z*sin(angle), v.y, v.z*cos(angle)-v.x*sin(angle)
  );
}

// Returns vector rotated some degrees along the z axis
inline float3 rotateZ(const float3 &v, const float angle) {
  return float3(
    v.x*cos(angle)-v.y*sin(angle), v.x*sin(angle)+v.y*cos(angle), v.z
  );
}
} // namespace bMath

#endif
