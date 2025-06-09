#ifndef BMATH_QUATERNION
#define BMATH_QUATERNION

#include "vector.hpp"

namespace bMath {
    template<typename T>
    struct Quaternion {
        union {
            Vector<T,4> vec;
            struct {
                T x, y, z, w;
            };
        };

        Quaternion() {}

        // Construct quaternion directly from values
        Quaternion(T w, T x, T y, T z) : w(w), x(x), y(y), z(z) {}

        // Construct quaternion from axis and an angle
        Quaternion(T angle, Vector<T,3> axis) {
            axis.normalize();
            vec.w = cos(angle/2);
            auto s = sin(angle/2);
            vec.x = s*axis.x;
            vec.y = s*axis.y;
            vec.z = s*axis.z;
        }

        // Construct quaternion from xyz euler angles
        Quaternion(T x, T y, T z) {
            Quaternion<T> xq(x, Vector<T,3>(1,0,0));
            Quaternion<T> yq(y, Vector<T,3>(0,1,0));
            Quaternion<T> zq(z, Vector<T,3>(0,0,1));
            Quaternion<T> result = xq*yq*zq;
            vec = result.vec;
        }

        Quaternion(Vector<T,4> v) : vec(v) {}

        // Returns angle of rotation in radians
        T angle() const {
            return std::acos(vec.w)*2;
        }

        Vector<T,3> axis() const {
            T angle = (*this).angle();
            T s = sin(angle/2);
            return Vector<T,3>(vec.x/s,vec.y/s,vec.z/s);
        }

        T length() const {
            return sqrt(x*x+y*y+z*z+w*w);
        }

        void normalize() {
            vec.normalize();
        }

        static Quaternion<T> identity() {
            return Quaternion<T>(1,0,0,0);
        }

        Quaternion inverse() const {
            return Quaternion(w, -x, -y, -z);
        }
    };

    typedef Quaternion<float> quaternion;

    template<typename T>
    Quaternion<T> operator*(const Quaternion<T> &q, const Quaternion<T> &p) {
        return Quaternion<T>(
            q.w*p.w-q.x*p.x-q.y*p.y-q.z*p.z,
            q.w*p.x+q.x*p.w+q.y*p.z-q.z*p.y,
            q.w*p.y-q.x*p.z+q.y*p.w+q.z*p.x,
            q.w*p.z+q.x*p.y-q.y*p.x+q.z*p.w
        );
    }

    template<typename T>
    Quaternion<T> operator*(const Quaternion<T> &q, const float s) {
        return Quaternion<T>(q.w*s, q.x*s, q.y*s, q.z*s);
    }

    // Add a scaled vector representation of a rotation to a quaternion
    template<typename T>
    void operator+=(Quaternion<T> &q, const Vector<T,3> &v) {
        q = bMath::quaternion(
            q.w + (0.5) * (-v.x * q.x - v.y * q.y - v.z * q.z),
            q.x + (0.5) * (v.x * q.w + v.y * q.z - v.z * q.y),
            q.y + (0.5) * (v.y * q.w + v.z * q.x - v.x * q.z),
            q.z + (0.5) * (v.z * q.w + v.x * q.y - v.y * q.x)
        );
        q.normalize();
    }

    template <typename T>
    T dot(const Quaternion<T> &q, const Quaternion<T> &p) {
        return dot(q.vec, p.vec);
    }

    // Returns shortest rotation between two quaternions
    template<typename T>
    Quaternion<T> rotationBetween(const Quaternion<T> &from, const Quaternion<T> &to) {
        if (dot(to,from) < 0)
            return to*(from.inverse()*-1.0f);
        else
            return to*(from.inverse());
    }
}

#endif