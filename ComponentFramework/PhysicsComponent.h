#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "Component.h"
#include "Vector.h"
#include "Quaternion.h"
#include "Matrix.h"

using namespace MATH;

class PhysicsComponent : public Component
{
public:
	float mass = 1.0f;
	Matrix3 rotationalInertia;
	Vec3 pos;
	Vec3 vel;
	Vec3 accel;
	Quaternion orientation;
	Vec3 angularVel;
	Vec3 angularAcc;

	PhysicsComponent(Component* parent_) : Component(parent_) {};
	PhysicsComponent(Component* parent_, float mass_, Matrix3 rotationalInertia_,
		Vec3 pos_, Vec3 vel_, Vec3 accel_, Quaternion orientation_,
		Vec3 angularVel_, Vec3 angularAcc_)
		: Component(parent_), mass(mass_), rotationalInertia(rotationalInertia_),
		pos(pos_), vel(vel_), accel(accel_), orientation(orientation_),
		angularVel(angularVel_), angularAcc(angularAcc_) {};
	~PhysicsComponent() {};

	bool OnCreate() { return true; }
	void OnDestroy() {};
	void Update(const float deltaTime_) {};
	void Render() const {};
		
};
#endif

