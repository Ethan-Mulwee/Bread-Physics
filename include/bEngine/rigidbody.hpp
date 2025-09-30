#ifndef BENGINE_BODY
#define BENGINE_BODY

#include "smath.hpp"

namespace bEngine {
	class RigidBody {
		public:
			float inverseMass;
			smath::matrix3x3 inverseInertiaTensor;

			smath::vector3 position = {0.0f, 0.0f, 0.0f};
			smath::quaternion orientation = smath::quaternion{0.0f,0.0f,0.0f,1.0f};

			smath::vector3 linearVelocity = {0.0f, 0.0f, 0.0f};
			smath::vector3 angularVelocity = {0.0f, 0.0f, 0.0f};

			smath::vector3 forceAccum = {0.0f, 0.0f, 0.0f};
			smath::vector3 torqueAccum = {0.0f, 0.0f, 0.0f};
		public:
			RigidBody() {};

			smath::matrix4x4 getTransform() const {
				smath::matrix3x3 orientationMatrix = smath::matrix3x3_from_quaternion(orientation);
				smath::matrix4x4 transform = smath::matrix4x4_from_matrix3x3(orientationMatrix);
				transform[0][3] = position.x;
				transform[1][3] = position.y;
				transform[2][3] = position.z;

				return transform;
			}

            smath::matrix3x3 getInverseInteriaTensorWorld() const {
                using namespace smath;

                matrix3x3 orientationMatrix = matrix3x3_from_quaternion(orientation);
                return transpose(orientationMatrix)*inverseInertiaTensor*orientationMatrix;
            }

			float getMass() const {
				return 1.0f/inverseMass;
			}

			smath::vector3 positionToBodySpace(smath::vector3 worldPosition) {
				worldPosition -= position;
				smath::matrix3x3 inverseOrientation = transpose(smath::matrix3x3_from_quaternion(orientation));
				return smath::matrix3x3_transform_vector3(inverseOrientation, worldPosition);
			}

			void addForce(const smath::vector3 &force);

			void addTorque(const smath::vector3 &torque);

			void addForceAtPoint(const smath::vector3 &force, const smath::vector3 &point);

			void addForceAtBodyPoint(const smath::vector3 &force, const smath::vector3 &point);

			void integrate(float time);
            
		private:
			void clearAccumlators();
    };
}

#endif