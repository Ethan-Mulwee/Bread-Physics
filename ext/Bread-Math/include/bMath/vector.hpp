#ifndef BMATH_VECTOR
#define BMATH_VECTOR

#include "float.hpp"
#include "fwd.hpp"
#include <algorithm>
#include <math.h>

namespace bMath {
template <typename T, std::size_t N> struct Vector {
    T data[N];

    template <typename... Args> Vector(Args... args) : data{(T)args...} {}

    T &operator[](int i) { return data[i]; }

    T operator[](int i) const { return data[i]; }

    float length() {
        float total = 0;
        for (int i = 0; i < N; i++) {
            total += data[i] * data[i];
        }
        return sqrt(total);
    }

    float squareLength () {
        float total = 0;
        for (int i = 0; i < N; i++) {
                total += data[i]*data[i];
        }
        return total;
    }

    // Normalize the vector
    void normalize() {
        float len = (*this).length();
        for (int i = 0; i < N; i++) {
            data[i] = data[i] * (float)1 / len;
        }
    }
};

template <typename T> struct Vector<T, 2> {
    union {
        T data[2];
        struct {
            T x, y;
        };
    };

    // TODO: Add a marco to automatically create these duplicate defs
    
    // TODO: Constructor to create vectors from smaller vectors like a vec3 from a vec2

    template <typename... Args> Vector(Args... args) : data{(T)args...} {}

    T &operator[](int i) { return data[i]; }

    T operator[](int i) const { return data[i]; }

    T length() const {
        T total = 0;
        for (int i = 0; i < 2; i++) {
            total += data[i] * data[i];
        }
        return (T)sqrt(total);
    }

    // Normalize the vector
    void normalize() {
        float len = (*this).length();
        for (int i = 0; i < 2; i++) {
            data[i] = (T)(data[i] * (float)1 / len);
        }
    }
};

template <typename T> struct Vector<T, 3> {
    union {
        T data[3];
        struct {
            T x, y, z;
        };
        Vector<T,2> xy;
    };

    template <typename... Args> Vector(Args... args) : data{(T)args...} {}

    T &operator[](int i) { return data[i]; }

    T operator[](int i) const { return data[i]; }

    T length() const {
        T total = 0;
        for (int i = 0; i < 3; i++) {
            total += data[i] * data[i];
        }
        return (T)sqrt(total);
    }

    T squareLength() const {
        T total = 0;
        for (int i = 0; i < 3; i++) {
            total += data[i]*data[i];
        }
        return (T)total;
    }

    // Normalize the vector
    void normalize() {
        T len = (*this).length();
        for (int i = 0; i < 3; i++) {
            data[i] = data[i] * (T)1 / len;
        }
    }
};

template <typename T> struct Vector<T, 4> {
    union {
        T data[4];
        struct {
            T x, y, z, w;
        };
        Vector<T,3> xyz;
    };

    template <typename... Args> Vector(Args... args) : data{(T)args...} {}

    T &operator[](int i) { return data[i]; }

    T operator[](int i) const { return data[i]; }

    T length() const {
        T total = 0;
        for (int i = 0; i < 4; i++) {
            total += data[i] * data[i];
        }
        return (T)sqrt(total);
    }

    // Normalize the vector
    void normalize() {
        T len = (*this).length();
        for (int i = 0; i < 4; i++) {
            data[i] = data[i] * (T)1 / len;
        }
    }
};

typedef Vector<float, 2> vector2;
typedef Vector<float, 3> vector3;
typedef Vector<float, 4> vector4;

typedef Vector<double, 2> double2;
typedef Vector<double, 3> double3;
typedef Vector<double, 4> double4;

typedef Vector<float, 2> float2;
typedef Vector<float, 3> float3;
typedef Vector<float, 4> float4;

typedef Vector<int, 2> int2;
typedef Vector<int, 3> int3;
typedef Vector<int, 4> int4;

template <typename T, std::size_t N>
Vector<T, N> operator+(const Vector<T, N> &a, const Vector<T, N> &b) {
    Vector<T, N> result;
    for (int i = 0; i < N; i++) {
        result[i] = a[i] + b[i];
    }
    return result;
}

template <typename T, std::size_t N>
void operator+=(Vector<T, N> &a, const Vector<T, N> &b) {
    for (int i = 0; i < N; i++) {
        a[i] += b[i];
    }
}

template <typename T, std::size_t N>
Vector<T, N> operator-(const Vector<T, N> &a, const Vector<T, N> &b) {
    Vector<T, N> result;
    for (int i = 0; i < N; i++) {
        result[i] = a[i] - b[i];
    }
    return result;
}

template <typename T, std::size_t N>
void operator-=(Vector<T, N> &a, const Vector<T, N> &b) {
    for (int i = 0; i < N; i++) {
        a[i] -= b[i];
    }
}

// Returns vector scaled by scalar
template <typename T, std::size_t N>
Vector<T, N> operator*(const Vector<T, N> &a, const float b) {
    Vector<T, N> result;
    for (int i = 0; i < N; i++) {
        result[i] = a[i] * b;
    }
    return result;
}

// Returns vector scaled by scalar
template <typename T, std::size_t N>
Vector<T, N> operator*(const float a, const Vector<T, N> &b) {
    Vector<T, N> result;
    for (int i = 0; i < N; i++) {
        result[i] = a * b[i];
    }
    return result;
}

// Returns vector divided by scalar component wise
template <typename T, std::size_t N>
Vector<T, N> operator/(const Vector<T, N> &a, const float b) {
    Vector<T, N> result;
    for (int i = 0; i < N; i++) {
        result[i] = a[i] / b;
    }
    return result;
} 

// Returns vector multipled component wise (hadamard product)
template <typename T, std::size_t N>
Vector<T, N> operator*(const Vector<T,N> &a, const Vector<T,N> &b) {
    Vector<T,N> result;
    for (int i = 0; i < N; i++) {
        result[i] = a[i]*b[i];
    }
}

// Multiples vector component wise by another vector
template <typename T, std::size_t N>
Vector<T, N> operator*=(Vector<T,N> &a, const Vector<T,N> &b) {
    for (int i = 0; i < N; i++) {
        a[i] *= b[i];
    }
}

// Multiples vector by a scalar
template <typename T, std::size_t N>
Vector<T, N> operator*=(Vector<T,N> &v, const T &s) {
    for (int i = 0; i < N; i++) {
        v[i] *= s;
    }
}

// Transforms vector by a matrix (assuming column vector)
template <typename T, std::size_t N, int rows>
Vector<T, rows> operator*(const Vector<T,N> &v, const Matrix<T,rows,N> &m) {
    Vector<T, rows> result;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < N; j++) {
            result[i] += v[j]*m(i,j);
        }
    }
    return result;
}

// TODO: make this less slow?
// Transforms vector by a matrix bigger than the source vector (just fills 1s) (assuming column vector)
template <typename T, std::size_t N, int cols, int rows>
Vector<T, N> operator*(const Vector<T,N> &v, const Matrix<T,rows,cols> &m) {
    Vector<T, N> result;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (i >= N) return result;
            T cord;
            if (j < N) cord = v[j];
            else cord = 1;
            result[i] += cord*m(i,j);
        }
    }
    return result;
}

// Returns vector transformed by matrix (will shrink dimension of vector of transformation does so)
// template <typename T, std::size_t n, int cols>
// Vector<T, cols> transform(const Vector<T,n> &v, const Matrix<T,n,cols> &m) {
//   Vector<T, cols> result;
// }

template <typename T, std::size_t N>
float lengthSquared(const Vector<T,N> &v) {
    float total;
    for (int i = 0; i < N; i++) {
        total += v[i]*v[i];
    }
    return total;
} 

template <typename T, std::size_t N>
float dot(const Vector<T, N> &a, const Vector<T, N> &b) {
    float result = 0;
    for (int i = 0; i < N; i++) {
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
    return normalized(Quaternion);
}

template <typename T>
Vector<T, 3> cross(const Vector<T, 3> &a, const Vector<T, 3> &b) {
    return Vector<T, 3>(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

template <typename T>
Vector<T, 3> project(const Vector<T, 3> &a, const Vector<T, 3> &b) {
    return (dot(a,b)/dot(b,b))*b;
}

template <typename T>
Vector<T, 3> projectOnPlane(const Vector<T, 3> &v, const Vector<T, 3> &n) {
    return v-project(v,n);
}

template <typename T, std::size_t N>
Vector<T, N> max(const Vector<T, N> &a, const Vector<T, N> &b) {
    Vector<T, N> result;
    for (int i = 0; i < N; i++) {
        result[i] = std::max(a[i], b[i]);
    }
    return result;
}

template <typename T, std::size_t N>
Vector<T, N> min(const Vector<T, N> &a, const Vector<T, N> &b) {
    Vector<T, N> result;
    for (int i = 0; i < N; i++) {
        result[i] = std::min(a[i], b[i]);
    }
    return result;
}

// template <typename T, std::size_t N>
// Vector<T, N> abs(const Vector<T, N> &v) {
//     Vector<T, N> result;
//     for (int i = 0; i < N; i++) {
//         result[i] = std::abs(v[i]);
//     }
//     return result;
// }

template <typename T, std::size_t N>
Vector<T, N> lerp(const Vector<T, N> &a, const Vector<T, N> &b, const float t) {
    Vector<T, N> result;
    for (int i = 0; i < N; i++) {
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
template <typename T, std::size_t N>
Vector<T, N> clamp(const Vector<T, N> &a, float b) {
    return Vector<T,N>();
}

// TODO
// Clamp vector component wise
template <typename T, std::size_t N>
Vector<T, N> clamp(const Vector<T, N> &a, const Vector<T, N>) {
    // std::clamp()
}

template <typename T, std::size_t N>
float distance(const Vector<T, N> &a, const Vector<T, N> &b) {
    return (a - b).length();
}

template <typename T, std::size_t N> Vector<T, N> normalized(const Vector<T, N> &a) {
    T len = a.length();
    Vector<T, N> result;
    for (int i = 0; i < N; i++) {
        result[i] = a[i] * (T)1 / len;
    }
    return result;
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
