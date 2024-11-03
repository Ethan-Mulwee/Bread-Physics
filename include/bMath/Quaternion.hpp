#ifndef BMATH_QUATERNION
#define BMATH_QUATERNION

#include <math.h>
#include <iostream>
#include "Vector.hpp"
#include "Matrix.hpp"

namespace bMath {
    struct Quaternion {
        float w,x,y,z;

        Quaternion() {}

        Quaternion(float w, float x, float y, float z)
        : w(w), x(x), y(y), z(z) {}

        Quaternion(float angle, Vector3 axis) {
            w = cos(angle/2);
            axis.normalize();
            x = sin(angle/2)*axis.x;
            y = sin(angle/2)*axis.y;
            z = sin(angle/2)*axis.z;
        }

        Quaternion operator+(const Quaternion &q) const {
            return Quaternion(w+q.w,x+q.x,y+q.y,z+q.z);
        }

        void operator+=(const Quaternion &q) {
            w += q.w;
            x += q.x;
            y += q.y;
            z += q.z;
        }

        Quaternion operator*(const Quaternion &q) const {
            return Quaternion();
        }

        Quaternion operator*(const float s) {
            return Quaternion(w*s,x*s,y*s,z*s);
        }
    
        void operator*=(const float s) {
            w *= s;
            x *= s;
            y *= s;
            z *= s;
        }

        float length() {
            return sqrt(w*w+x*x+y*y+z*z);
        }

        void normalize() {
            (*this) *= (float)(1/(*this).length());
        }

        void rotate(const Vector3 &v) {
            float _w = (0.5)*(-v.x*x-v.y*y-v.z*z);
            float _x = (0.5)*(v.x*w+v.y*z-v.z*y);
            float _y = (0.5)*(v.y*w+v.z*x-v.x*z);
            float _z = (0.5)*(v.z*w+v.x*y-v.y*x);
            Quaternion result = (*this) + Quaternion(_x,_y,_z,_w);
            result.normalize();
            (*this) = result;
        }

        Matrix<3,3> ToMatrix() {
            Matrix<3,3> m;
            // i hat
            m.data[0][0] = x*x-y*y-z*z+w*w;
            m.data[1][0] = 2*w*z+2*x*y;
            m.data[2][0] = 2*x*z-2*w*y;
            // j hat
            m.data[0][1] = 2*x*y-2*w*z;
            m.data[1][1] = w*w-x*x+y*y-z*z;
            m.data[2][1] = 2*w*x+2*y*z;
            // k hat
            m.data[0][2] = 2*x*z+2*w*y;
            m.data[1][2] = 2*y*z-2*w*x;
            m.data[2][2] = w*w-x*x-y*y+z*z;

            return m;
        }

        void log() {
            std::cout << w << ", " << x << ", " << y << ", " << z << "\n";
        }
    };

    Quaternion QuaternionFromAxisAngle(float angle, Vector3 axis) {
        axis.normalize();
        float a = sin(angle/2);
        return Quaternion(cos(angle/2), a*axis.x, a*axis.y, a*axis.z);
    }

    std::ostream& operator<<(std::ostream& os, const Quaternion &q) {
        os << "(" << q.w << q.x << ", " << q.y << ", " << q.z << ")";
        return os;
    }
}

#endif