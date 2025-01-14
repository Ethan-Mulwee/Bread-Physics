#ifndef BENGINE_CONTACTS
#define BENGINE_CONTACTS

#include <bMath/bMath.hpp>

#include "rigidbody.hpp"

namespace bEngine {
    enum PrimitiveType {
        Sphere,
        Cube,
        Plane
    };

    struct Primitive {
        RigidBody* body;
        PrimitiveType type;
        bMath::matrix4 offset;
        float height;
        float width;

        bMath::float4x4 getTransform() const {
            return body->getTransform();
        }
    };

    struct PenetrationResolutionResult {
        bMath::float3 linearChange[2];
        bMath::float3 angularChange[2];
    };

    struct Contact {
        RigidBody* body[2] = { nullptr, nullptr };
        float friction;
        float restitution;
        bMath::float3 contactPoint;
        bMath::float3 contactNormal;
        float penetration;

        bMath::matrix3 getContactBasis() const;

        float getClosingVelocity() const; 

        inline bMath::float3 getContactPointRelativeToBody(unsigned bodyIndex) const {
            return contactPoint - body[bodyIndex]->position;
        }

        void resolveVelocity();

        PenetrationResolutionResult resolvePenetration();
    };

    class ContactPool {
        private:
            const static unsigned maxContacts = 256;
            Contact contacts[256];
            Contact* contactPtr = contacts;
            int contactsLeft = maxContacts;
            unsigned contactCount = 0;
    
            float friction = 0.2f;
            float restitution = 0.2f;

        public:
            void push(Contact contact) {
                contactsLeft--;
                contactCount++;
    
                *contactPtr = contact;
    
                contactPtr++;
            }

            unsigned room() {
                return contactsLeft;
            }

            unsigned count() {
                return contactCount;
            }

            unsigned capacity() {
                return maxContacts;
            }

            // Contact operator[](const int i) const { return contacts[i]; }
            Contact& operator[](const int i) { return contacts[i]; }

            void reset() {
                contactPtr = contacts;
                contactsLeft = maxContacts;
                contactCount = 0;
            }
    };
}

#endif