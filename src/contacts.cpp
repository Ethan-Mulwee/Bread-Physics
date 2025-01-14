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



PenetrationResolutionResult Contact::resolvePenetration() {
    using namespace bMath;

    float3 bodyPoint[2]; 
    bodyPoint[0] = contactPoint - body[0]->position;
    if (body[1]) bodyPoint[1] = contactPoint - body[1]->position;

    float angularMove[2];
    float linearMove[2];

    float angularInverseInertia[2];
    float linearInverseInertia[2];
    float totalInverseInertia = 0;

    PenetrationResolutionResult result;

    for (unsigned i = 0; i < 2; i++) if (body[i]) {
        float3 angularInverseInertiaWorld = cross(bodyPoint[i], contactNormal);
        angularInverseInertiaWorld = angularInverseInertiaWorld*body[i]->getInverseInteriaTensorWorld();
        angularInverseInertiaWorld = cross(angularInverseInertiaWorld, bodyPoint[i]);

        angularInverseInertia[i] = dot(angularInverseInertiaWorld,contactNormal);

        linearInverseInertia[i] = body[i]->inverseMass;

        totalInverseInertia += linearInverseInertia[i] + angularInverseInertia[i];
    }

    for (unsigned i = 0; i < 2; i++) if (body[i]) {
        float sign = (i == 0) ? 1 : -1;

        angularMove[i] = sign * penetration * (angularInverseInertia[i] / totalInverseInertia);
        linearMove[i] = sign * penetration * (linearInverseInertia[i] / totalInverseInertia);

        // Angular move limit proportional to body size to prevent over-rotation
        float limit = 0.2f * bodyPoint[i].length();
        if (std::abs(angularMove[i]) > limit) {
            float totalMove = linearMove[i] + angularMove[i];

            if (angularMove[i] >= 0 ) angularMove[i] = limit;
            else angularMove[i] = -limit;

            linearMove[i] = totalMove - angularMove[i];
        } 

        result.angularChange[i] = cross(bodyPoint[i],contactNormal)*body[i]->getInverseInteriaTensorWorld()*(angularMove[i]/angularInverseInertia[i]);
        body[i]->orientation += result.angularChange[i];

        result.linearChange[i] = contactNormal * linearMove[i];
        body[i]->position += result.linearChange[i];
    }

    return result;
}

void bEngine::Contact::resolveVelocity() {
    using namespace bMath;

    float3 bodyPoint[2]; 
    bodyPoint[0] = contactPoint - body[0]->position;
    if (body[1]) bodyPoint[1] = contactPoint - body[1]->position;
    matrix3 contactToWorld = transpose(getContactBasis());
    float closingVelocity = getClosingVelocity();
    const float restitution = 0.5f;

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