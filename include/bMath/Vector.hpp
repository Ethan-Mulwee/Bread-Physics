#ifndef BMATH_VECTOR
#define BMATH_VECTOR

#include <math.h>
#include <iostream>
#include "Float.hpp"

namespace bMath {
    struct Vector3 {
        float x,y,z;

        Vector3() : x(0), y(0), z(0) {}

        Vector3(float x,float y,float z) : x(x), y(y), z(z) {}

        Vector3 operator+(const Vector3 &v) const {
            return Vector3(x+v.x,y+v.y,z+v.z);
        }
        void operator+=(const Vector3 &v) {
            x += v.x;
            y += v.y;
            z += v.z;
        }

        Vector3 operator*(const float s) const {
            return Vector3(x*s,y*s,z*s);
        }
        void operator*=(const float s) {
            x *= s;
            y *= s;
            z *= s;
        }

        Vector3 operator-(const Vector3 &v) const {
            return Vector3(x-v.x,y-v.y,z-v.z);
        }

        void operator-=(const Vector3 &v) {
            x -= v.x;
            y -= v.y;
            z -= v.z;
        }


        Vector3 cross(const Vector3 &v) const {
            return Vector3(
                y*v.z-z*v.y,
                z*v.x-x*v.z,
                x*v.y-y*v.x
            );
        }
        
        float dot(const Vector3 &v) const {
            return x*v.x+y*v.y+z*v.z;
        }

        float magnitude() const {
            return sqrt(x*x+y*y+z*z);
        }
        float squareMagnitude() const {
            return (x*x+y*y+z*z);
        }

        void normalize() {
            float length = magnitude();
            if (length > 0) {
                (*this) *= ((float)1/length);
            }
        }

    };

    std::ostream& operator<<(std::ostream& os, const Vector3 &v) {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;
    }

    float dot(Vector3 a, Vector3 b) {
        return  a.x*b.x+a.y*b.y+a.z*b.z;
    }

    Vector3 cross(Vector3 a, Vector3 b) {
        return Vector3(
                a.y*b.z-a.z*b.y,
                a.z*b.x-a.x*b.z,
                a.x*b.y-a.y*b.x
        );
    }

    Vector3 max(Vector3 a, Vector3 b) {
        float x = std::max(a.x,b.x);
        float y = std::max(a.y,b.y);
        float z = std::max(a.z,b.z);
        return Vector3(x,y,z);
    }

    Vector3 min(Vector3 a, Vector3 b) {
        float x = std::min(a.x,b.x);
        float y = std::min(a.y,b.y);
        float z = std::min(a.z,b.z);
        return Vector3(x,y,z);
    }

    Vector3 lerp(Vector3 a, Vector3 b, float t) {
        float x = lerp(a.x,b.x,t);
        float y = lerp(a.y,b.y,t);
        float z = lerp(a.z,b.z,t);
        return Vector3(x,y,z);
    }

    float distance(Vector3 a, Vector3 b) {
        return std::abs((a-b).magnitude());
    }



    //TODO
    Vector3 clamp(Vector3 a, Vector3 b) {
        return Vector3();
    }
}

#endif