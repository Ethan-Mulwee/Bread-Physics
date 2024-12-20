#ifndef BENGINE_BODY
#define BENGINE_BODY

#include <bMath/bMath.hpp>

namespace bEngine {
	class RigidBody {
		public:
			float inverseMass;
			bm::float3x3 inverseInertiaTensor;

			bm::float3 position;
			bm::quaternion orientation = bm::quaternion(1,0,0,0);

			bm::float3 linearVelocity;
			bm::float3 angularVelocity;

			bm::float3 forceAccum;
			bm::float3 torqueAccum;
            

		public:
			RigidBody() {};

			bm::float4x4 getTransform() {
				bm::float3x3 o = quaternionToMatrix(orientation);
				return bm::matrix4(
					o(0,0), o(0,1), o(0,2), position.x,
					o(1,0), o(1,1), o(1,2), position.y,
					o(2,0), o(2,1), o(2,2), position.z,
					0,      0,      0,      1
				);
			}

			bm::float3 positionToBodySpace(bm::float3 wpos) {
				wpos -= position;
				bm::float3x3 o = quaternionToMatrix(orientation);
				o = bm::transpose(o);
				return wpos*o;
			}

			void addForce(const bm::float3 &force);

			void addTorque(const bm::float3 &torque);

			void addForceAtPoint(const bm::float3 &force, const bm::float3 &point);

			void addForceAtBodyPoint(const bm::float3 &force, const bm::float3 &point);

			void integrate(float time);
            
		private:
			void clearAccumlators();
    };
}

#endif