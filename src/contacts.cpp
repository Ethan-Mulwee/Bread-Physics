#include "bEngine/contacts.hpp"

using namespace bEngine;

smath::matrix3x3 Contact::getContactBasis() const {
    using namespace smath;

    vector3 tagentY;
    vector3 tagentZ;

    if (std::abs(contactNormal.y) < std::abs(contactNormal.z)) {
        tagentZ = vector3{-contactNormal.z, 0, contactNormal.x};
        tagentY = cross(contactNormal, tagentZ);
    }
    else {
        tagentY = vector3{-contactNormal.y, contactNormal.x, 0};
        tagentZ = cross(contactNormal, tagentY);
    }

    return smath::matrix3x3{
        contactNormal.x, tagentY.x, tagentZ.x,
        contactNormal.y, tagentY.y, tagentZ.y,
        contactNormal.z, tagentY.z, tagentZ.z
    };
}

float Contact::getClosingVelocity() const {
    using namespace smath;

    vector3 bodyPoint1 = contactPoint - body[0]->position;
    vector3 velocity = cross(body[0]->angularVelocity, bodyPoint1);
    velocity += body[0]->linearVelocity;

    if (body[1]) {
        vector3 bodyPoint2 = contactPoint - body[1]->position;
        velocity -= cross(body[1]->angularVelocity, bodyPoint2);
        velocity -= body[1]->linearVelocity;
    }

    velocity = matrix3x3_transform_vector3(getContactBasis(), velocity);
    
    return velocity.x;
}

// Returns velocity between the bodies relative to contact space
smath::vector3 Contact::getRelativeVelocity() const {
    using namespace smath;
    
    vector3 bodyPoint1 = contactPoint - body[0]->position;
    vector3 velocity = cross(body[0]->angularVelocity, bodyPoint1);
    velocity += body[0]->linearVelocity;

    if (body[1]) {
        vector3 bodyPoint2 = contactPoint - body[1]->position;
        velocity -= cross(body[1]->angularVelocity, bodyPoint2);
        velocity -= body[1]->linearVelocity;
    }

    velocity = matrix3x3_transform_vector3(getContactBasis(), velocity);
    
    return velocity;
}


PenetrationResolutionResult Contact::resolvePenetration() {
    using namespace smath;

    vector3 bodyPoint[2]; 
    bodyPoint[0] = contactPoint - body[0]->position;
    if (body[1]) bodyPoint[1] = contactPoint - body[1]->position;

    float angularMove[2];
    float linearMove[2];

    float angularInverseInertia[2];
    float linearInverseInertia[2];
    float totalInverseInertia = 0;

    PenetrationResolutionResult result;

    for (unsigned i = 0; i < 2; i++) if (body[i]) {
        vector3 angularInverseInertiaWorld = cross(bodyPoint[i], contactNormal);
        angularInverseInertiaWorld = matrix3x3_transform_vector3(body[i]->getInverseInteriaTensorWorld(), angularInverseInertiaWorld);
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

        result.angularChange[i] = matrix3x3_transform_vector3(body[i]->getInverseInteriaTensorWorld(), cross(bodyPoint[i],contactNormal)) * (angularMove[i]/angularInverseInertia[i]);
        body[i]->orientation = quaternion_add_vector(body[i]->orientation, result.angularChange[i]);

        result.linearChange[i] = contactNormal * linearMove[i];
        body[i]->position += result.linearChange[i];
    }

    return result;
}

void bEngine::Contact::resolveVelocity() {
    using namespace smath;

    vector3 bodyPoint[2]; 
    bodyPoint[0] = contactPoint - body[0]->position;
    if (body[1]) bodyPoint[1] = contactPoint - body[1]->position;
    matrix3x3 contactToWorld = transpose(getContactBasis());
    // TODO: temporarily hard-coded
    // TODO: add static friction
    float restitution = 0.5f;
    float static_friction = 0.5f;
    float kinetic_friction = 0.4f;

    vector3 relativeVelocity = getRelativeVelocity();
    float closingVelocity = relativeVelocity.x;

    const vector3 contactBasis[3] = {contactNormal, vector3_from_matrix3x3(contactToWorld, 1), vector3_from_matrix3x3(contactToWorld, 2)};
    vector3 angularInverseInertiaWorld[3];

    for (int i = 0; i < 3; i++) {
        angularInverseInertiaWorld[i] = cross(bodyPoint[0], contactBasis[i]);
        angularInverseInertiaWorld[i] = matrix3x3_transform_vector3(body[0]->getInverseInteriaTensorWorld(), angularInverseInertiaWorld[i]);
        angularInverseInertiaWorld[i] = cross(angularInverseInertiaWorld[i], bodyPoint[0]);
    }

    float inverseInertia[3];
    for (int i = 0; i < 3; i++) {
        inverseInertia[i] = dot(angularInverseInertiaWorld[i],contactBasis[i]);
        inverseInertia[i] += body[0]->inverseMass;
    }

    if (body[1]) {
        vector3 angularInverseInertiaWorld[3];

        for (int i = 0; i < 3; i++) {
            angularInverseInertiaWorld[i] = cross(bodyPoint[1], contactBasis[i]);
            angularInverseInertiaWorld[i] = matrix3x3_transform_vector3(body[1]->getInverseInteriaTensorWorld(), angularInverseInertiaWorld[i]);
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

    vector3 planarImpluse{0,-relativeVelocity.y/inverseInertia[1], -relativeVelocity.z/inverseInertia[2]};
    if (planarImpluse.length() > impluseFriction)
        planarImpluse = normalized(planarImpluse)*impluseFriction;
        
    vector3 impluse{impluseNormal, 0.0f, 0.0f};
    impluse += planarImpluse;

    impluse = matrix3x3_transform_vector3(contactToWorld, impluse);

    vector3 implusiveTorque[2]; 
    implusiveTorque[0] = cross(bodyPoint[0], impluse);
    if (body[1]) implusiveTorque[1] = cross(bodyPoint[1], -1.0f*impluse);

    body[0]->linearVelocity += impluse*body[0]->inverseMass;
    body[0]->angularVelocity += matrix3x3_transform_vector3(body[0]->getInverseInteriaTensorWorld(), implusiveTorque[0]);
    
    if (body[1]) {
        body[1]->linearVelocity += impluse*-body[1]->inverseMass;
        body[1]->angularVelocity += matrix3x3_transform_vector3(body[1]->getInverseInteriaTensorWorld(), implusiveTorque[1]);
    }
}