#include "bEngine/contacts.hpp"

using namespace bEngine;

bMath::matrix3 Contact::getContactBasis() const {
    using namespace bMath;

    float3 tagentY;
    float3 tagentZ;

    if (abs(contactNormal.y) < abs(contactNormal.z)) {
        tagentZ = float3(-contactNormal.z, 0, contactNormal.x);
        tagentY = cross(contactNormal, tagentZ);
    }
    else {
        tagentY = float3(-contactNormal.y, contactNormal.x, 0);
        tagentZ = cross(contactNormal, tagentY);
    }

    return bMath::matrix3(
        contactNormal.x, tagentY.x, tagentZ.x,
        contactNormal.y, tagentY.y, tagentZ.y,
        contactNormal.z, tagentY.z, tagentZ.z
    );
}

float Contact::getClosingVelocity() const {
    using namespace bMath;
    float3 bodyPoint1 = contactPoint - body[0]->position;
    float3 velocity = cross(body[0]->angularVelocity, bodyPoint1);
    velocity += body[0]->linearVelocity;

    if (body[1]) {
        float3 bodyPoint2 = contactPoint - body[1]->position;
        velocity -= cross(body[1]->angularVelocity, bodyPoint2);
        velocity -= body[1]->linearVelocity;
    }

    velocity = velocity * getContactBasis();
    
    return velocity.x;
}

// NOTE: debugging only
#include <iostream>
#include <bMath/ext/iostream.hpp>
#include <bMath/ext/raylib.hpp>

void bEngine::Contact::resolvePenetration() {
    using namespace bMath;

    // std::cout << "penetration:" << penetration << "\n";

    body[0]->position += contactNormal*penetration;
}

void bEngine::Contact::resolveVelocity() {
    using namespace bMath;

    float3 bodyPoint = contactPoint - body[0]->position;
    matrix3 contactToWorld = transpose(getContactBasis());
    float closingVelocity = getClosingVelocity();
    const float restitution = 0.0f;

    // Get angularInverseInteria
    float3 inverseInertiaWorld = cross(bodyPoint, contactNormal);
    inverseInertiaWorld = inverseInertiaWorld*body[0]->getInverseInteriaTensorWorld();
    inverseInertiaWorld = cross(inverseInertiaWorld, bodyPoint);

    float inverseInertia = dot(inverseInertiaWorld,contactNormal);
    inverseInertia += body[0]->inverseMass;

    float3 impluse(-closingVelocity*(1+restitution) / inverseInertia);
    impluse = impluse*contactToWorld;

    float3 implusiveTorque = cross(bodyPoint, impluse);

    body[0]->linearVelocity += impluse*body[0]->inverseMass;
    body[0]->angularVelocity += implusiveTorque*body[0]->getInverseInteriaTensorWorld();
}