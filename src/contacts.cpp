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
    penetration = 0.0f;
}

void bEngine::Contact::resolveVelocity() {
    using namespace bMath;

    float3 bodyPoint[2]; 
    bodyPoint[0] = contactPoint - body[0]->position;
    if (body[1]) bodyPoint[1] = contactPoint - body[1]->position;
    matrix3 contactToWorld = transpose(getContactBasis());
    float closingVelocity = getClosingVelocity();
    const float restitution = 0.0f;

    float3 angularInverseInertiaWorld = cross(bodyPoint[0], contactNormal);
    angularInverseInertiaWorld = angularInverseInertiaWorld*body[0]->getInverseInteriaTensorWorld();
    angularInverseInertiaWorld = cross(angularInverseInertiaWorld, bodyPoint[0]);

    float inverseInertia = dot(angularInverseInertiaWorld,contactNormal);
    inverseInertia += body[0]->inverseMass;

    if (body[1]) {
        float3 angularInverseInertiaWorld = cross(bodyPoint[1], contactNormal);
        angularInverseInertiaWorld = angularInverseInertiaWorld*body[1]->getInverseInteriaTensorWorld();
        angularInverseInertiaWorld = cross(angularInverseInertiaWorld, bodyPoint[1]);

        inverseInertia += dot(angularInverseInertiaWorld,contactNormal);
        inverseInertia += body[1]->inverseMass;
    }

    float3 impluse(-closingVelocity*(1+restitution) / inverseInertia);
    impluse = impluse*contactToWorld;

    float3 implusiveTorque[2]; 
    implusiveTorque[0] = cross(bodyPoint[0], impluse);
    if (body[1]) implusiveTorque[1] = cross(bodyPoint[1], impluse);

    body[0]->linearVelocity += impluse*body[0]->inverseMass;
    body[0]->angularVelocity += implusiveTorque[0]*body[0]->getInverseInteriaTensorWorld();

    if (body[1]) {
        body[1]->linearVelocity += impluse*body[1]->inverseMass;
        body[1]->angularVelocity += implusiveTorque[1]*body[1]->getInverseInteriaTensorWorld();
    }
}