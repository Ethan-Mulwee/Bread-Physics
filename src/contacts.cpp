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

// Returns velocity between the bodies relative to contact space
bMath::float3 Contact::getRelativeVelocity() const {
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
    
    return velocity;
}


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
    // TODO: temporarily hard-coded
    // TODO: add static friction
    float restitution = 0.5f;
    float static_friction = 0.5f;
    float kinetic_friction = 0.4f;

    float3 relativeVelocity = getRelativeVelocity();
    float closingVelocity = relativeVelocity.x;

    const float3 contactBasis[3] = {contactNormal, contactToWorld.col(1), contactToWorld.col(2)};
    float3 angularInverseInertiaWorld[3];

    for (int i = 0; i < 3; i++) {
        angularInverseInertiaWorld[i] = cross(bodyPoint[0], contactBasis[i]);
        angularInverseInertiaWorld[i] = angularInverseInertiaWorld[i]*body[0]->getInverseInteriaTensorWorld();
        angularInverseInertiaWorld[i] = cross(angularInverseInertiaWorld[i], bodyPoint[0]);
    }

    float inverseInertia[3];
    for (int i = 0; i < 3; i++) {
        inverseInertia[i] = dot(angularInverseInertiaWorld[i],contactBasis[i]);
        inverseInertia[i] += body[0]->inverseMass;
    }

    if (body[1]) {
        float3 angularInverseInertiaWorld[3];

        for (int i = 0; i < 3; i++) {
            angularInverseInertiaWorld[i] = cross(bodyPoint[1], contactBasis[i]);
            angularInverseInertiaWorld[i] = angularInverseInertiaWorld[i]*body[1]->getInverseInteriaTensorWorld();
            angularInverseInertiaWorld[i] = cross(angularInverseInertiaWorld[i], bodyPoint[1]);
        }

        for (int i = 0; i < 3; i++) {
            inverseInertia[i] += dot(angularInverseInertiaWorld[i],contactBasis[i]);
            inverseInertia[i] += body[1]->inverseMass;
        }
    }

    if (closingVelocity < 0.5f) {
        restitution = 0.0f;
    }

    float impluseNormal = -(closingVelocity)*(1+restitution) / inverseInertia[0];
    float impluseFriction = -impluseNormal * kinetic_friction;

    float3 planarImpluse(0,-relativeVelocity.y/inverseInertia[1], -relativeVelocity.z/inverseInertia[2]);
    if (planarImpluse.length() > impluseFriction)
        planarImpluse = normalized(planarImpluse)*impluseFriction;
        
    float3 impluse(impluseNormal, 0.0f, 0.0f);
    impluse += planarImpluse;

    impluse = impluse*contactToWorld;

    float3 implusiveTorque[2]; 
    implusiveTorque[0] = cross(bodyPoint[0], impluse);
    if (body[1]) implusiveTorque[1] = cross(bodyPoint[1], -1.0f*impluse);

    body[0]->linearVelocity += impluse*body[0]->inverseMass;
    body[0]->angularVelocity += implusiveTorque[0]*body[0]->getInverseInteriaTensorWorld();
    
    if (body[1]) {
        body[1]->linearVelocity += impluse*-body[1]->inverseMass;
        body[1]->angularVelocity += implusiveTorque[1]*body[1]->getInverseInteriaTensorWorld();
    }
}