#ifndef BENGINE_COLLIDE_NARROW
#define BENGINE_COLLIDE_NARROW

#include "contacts.hpp"
#include "rigidbody.hpp"
#include "smath.hpp"
#include <float.h>
#include <vector>

namespace bEngine {
    struct CollisionDetector {
        static void cubeFloor(const Primitive &cube, const float floorHeight, ContactPool &contacts);
        static void cubeCube (const Primitive &one, const Primitive &two, ContactPool &contacts);

    };


    static inline float transformToAxis(const Primitive &box, const smath::vector3 &axis) {
         return 
            box.dimensions.x * std::abs(dot(axis,box.getAxis(0))) +
            box.dimensions.y * std::abs(dot(axis,box.getAxis(1))) +
            box.dimensions.z * std::abs(dot(axis,box.getAxis(2)));
    }


    static inline float penetrationOnAxis(const Primitive &one, const Primitive &two, const smath::vector3 &axis, const smath::vector3 &toCenter) {
        float oneProjection = transformToAxis(one, axis);
        float twoProjection = transformToAxis(two, axis);

        float distance = std::abs(dot(toCenter, axis));

        return oneProjection + twoProjection - distance;
    }
}

#endif