#ifndef PHYSICS_H
#define PHYSICS_H

#include "PhysicsComponent.h"
#include "TransformComponent.h"
#include "MMath.h"
#include "QMath.h"

namespace PHYSICS {
	void ApplyTorque(Ref<PhysicsComponent> body, MATH::Vec3 torque);
	void ApplyForce(Ref<PhysicsComponent> body, MATH::Vec3 force);
	void ApplyAngularMotion(Ref<PhysicsComponent> body, const float deltaTime);
	void ApplyLinearMotion(Ref<PhysicsComponent> body, const float deltaTime);
	void UpdatePos(Ref<PhysicsComponent> body, const float deltaTime);
	void UpdateVel(Ref<PhysicsComponent> body, const float deltaTime);
	void UpdateTransform(const Ref<PhysicsComponent> body, Ref<TransformComponent> transform);
	void MouseConstraint(Ref<PhysicsComponent> body, float deltaTime, const Vec3& mousePos);

}
#endif
