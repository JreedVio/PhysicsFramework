#include "Physics.h"

void PHYSICS::ApplyTorque(Ref<PhysicsComponent> body, MATH::Vec3 torque)
{
	body->angularAcc = MATH::MMath::inverse(body->rotationalInertia) * torque;
}

void PHYSICS::ApplyForce(Ref<PhysicsComponent> body, MATH::Vec3 force)
{
	body->accel = force / body->mass;
}

void PHYSICS::ApplyAngularMotion(Ref<PhysicsComponent> body, const float deltaTime)
{
	body->angularVel += body->angularAcc * deltaTime;
	//we can rotate too with the mouse constraint, so update orientation too
	Quaternion angularVelQuaternion(0.0, body->angularVel);
	// Rotate using q = q + 0.5 twq
	body->orientation = body->orientation + angularVelQuaternion * body->orientation * 0.5f * deltaTime;
	// don't forget to normalize after too. Only unit quaternions please. Otherwise the model stretches
	body->orientation = MATH::QMath::normalize(body->orientation);
}

void PHYSICS::ApplyLinearMotion(Ref<PhysicsComponent> body, const float deltaTime)
{
	UpdatePos(body, deltaTime);
	UpdateVel(body, deltaTime);
}

void PHYSICS::UpdatePos(Ref<PhysicsComponent> body, const float deltaTime)
{
	body->pos += body->vel * deltaTime + 0.5f * body->accel * deltaTime * deltaTime; 
	//body->pos += body->vel * deltaTime;
}

void PHYSICS::UpdateVel(Ref<PhysicsComponent> body, const float deltaTime)
{
	body->vel += body->accel * deltaTime;
}

void PHYSICS::UpdateTransform(const Ref<PhysicsComponent> body, Ref<TransformComponent> transform)
{
	transform->pos = body->pos;
	transform->orientation = body->orientation;
}

void PHYSICS::MouseConstraint(Ref<PhysicsComponent> body, float deltaTime, const Vec3& mousePos)
{
	// Assume the position in world space is the centre of mass
	MATH::Vec3 body_centreOfMass = body->pos;
	// r is the vector from the centre of mass to the attachment point
	MATH::Vec3 r = mousePos - body_centreOfMass;
	MATH::Matrix3 m_effective(
		1 + r.z * r.z + r.y * r.y, -r.x * r.y, -r.x * r.z,  // first column
		-r.x * r.y, 1 + r.z * r.z + r.x * r.x, -r.y * r.z,   // second column
		-r.x * r.z, -r.y * r.z, 1 + r.y * r.y + r.x * r.x); // third column

	MATH::Matrix3 m_effective_inversed = MATH::MMath::inverse(m_effective);
	// Now calculate lambda = m_effective_inversed *(-JV -b)
	// lambda = m_effective_inversed * (beta/deltatime) * d - m_effective_inversed * JV
	float beta = 0.2f;
	MATH::Vec3 d = (body_centreOfMass + r - mousePos);
	// at the start of the program deltaTime == zero! Set to 1/60 instead
	if (deltaTime < VERY_SMALL) { deltaTime = 0.01667f; }
	MATH::Vec3 negative_b = (beta / deltaTime) * d;
	// for a static anchor point I wrote out the JV vector on paper
	MATH::Vec3 JV = body->vel + MATH::VMath::cross(body->angularVel, r);;

	MATH::Vec3 lambda = m_effective_inversed * negative_b + m_effective_inversed * JV;
	// I wrote out on paper what the change in vel & angularVel should be
	MATH::Vec3 deltaAngularVel = -MATH::VMath::cross(lambda, r);

	body->vel -= lambda;
	body->angularVel -= deltaAngularVel;
}
