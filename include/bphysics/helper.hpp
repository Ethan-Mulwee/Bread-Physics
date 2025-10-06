#ifndef BPHYSICS_HELPER
#define BPHYSICS_HELPER

#include "contacts.hpp"

namespace bphys {
    RigidBody* createRigidBody(
        const smath::vector3 &position,
        const smath::quaternion &orientation,
        const float inverseMass,
        const smath::matrix3x3 &inertiaTensor
    ) {
        RigidBody* body = new RigidBody();
        body->position = position;
        body->orientation = orientation;

        body->linearVelocity = smath::vector3{0,0,0};
        body->angularVelocity = smath::vector3{0,0,0};

        body->inverseMass = inverseMass;
        body->inverseInertiaTensor = smath::inverse(inertiaTensor);

        return body;
    }

    Primitive createCollider(
        const PrimitiveType type,
        const smath::vector3 dimensions,
        const smath::matrix4x4 transform,
        RigidBody* body
    ) {
        Primitive collider;
        collider.type = type;
        collider.dimensions = dimensions;
        collider.offset = transform;
        collider.body = body;

        return collider;
    }
}

#endif