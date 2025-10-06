#ifndef BENGINE_CONTACTS
#define BENGINE_CONTACTS

#include "smath.hpp"

#include "rigidbody.hpp"

// #include <iostream>
// #include <cmath>

namespace bphys {
    enum PrimitiveType {
        Sphere,
        Cube,
        Plane
    };

    struct Primitive {
        RigidBody* body;
        PrimitiveType type;
        smath::matrix4x4 offset;
        smath::vector3 dimensions = {1.0f,1.0f,1.0f};

        inline smath::matrix4x4 getTransform() const {
            return body->getTransform();
        }

        inline smath::vector3 getAxis(const unsigned i) const {
            smath::matrix4x4 transform = getTransform();
            // if (std::isnan(transform(0,i)) || std::isnan(transform(1,i)) || std::isnan(transform(2,i))) {
            //     std::cout << "Nan detected \n";
            // }
            return smath::vector3{transform[0][i], transform[1][i], transform[2][i]};
        }
    };

    struct PenetrationResolutionResult {
        smath::vector3 linearChange[2] = {{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f}};
        smath::vector3 angularChange[2] = {{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f}};
    };

    struct Contact {
        RigidBody* body[2] = { nullptr, nullptr };
        float friction = 0.0f;
        float restitution = 0.0f;
        smath::vector3 contactPoint = {0.0f,0.0f,0.0f};
        smath::vector3 contactNormal = {0.0f,0.0f,0.0f};
        float penetration;

        const char* debugLabel = "";

        smath::matrix3x3 getContactBasis() const;

        float getClosingVelocity() const; 

        smath::vector3 getRelativeVelocity() const;

        inline smath::vector3 getContactPointRelativeToBody(unsigned bodyIndex) const {
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