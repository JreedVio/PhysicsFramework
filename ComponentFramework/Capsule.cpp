#include "Capsule.h"
#include "MMath.h"
#include "Matrix.h"
#include "QuadraticSolution.h"

using namespace MATH;
using namespace GEOMETRY;

GEOMETRY::Capsule::Capsule()
{
	r = 1.0f;
	sphereCentrePosA.set(0.0f, 1.0f, 0.0f);
	sphereCentrePosB.set(0.0f, 1.0f, 10.0f);
	generateVerticesAndNormals();
	StoreMeshData(GL_POINTS);
}

GEOMETRY::Capsule::Capsule(float r_, MATH::Vec3 sphereCentrePosA_, MATH::Vec3 sphereCentrePosB_)
{
	set(r_, sphereCentrePosA_, sphereCentrePosB_);
}

void GEOMETRY::Capsule::set(float r_, MATH::Vec3 sphereCentrePosA_, MATH::Vec3 sphereCentrePosB_)
{
	r = r_;
	sphereCentrePosA = sphereCentrePosA_;
	sphereCentrePosB = sphereCentrePosB_;
	generateVerticesAndNormals();
	StoreMeshData(GL_POINTS);
}

void GEOMETRY::Capsule::generateVerticesAndNormals()
{
	const float deltaTheta = 1.0f;
	const float cylinderLength = VMath::mag(sphereCentrePosB - sphereCentrePosA);
	const Vec3 axis = VMath::normalize(sphereCentrePosB - sphereCentrePosA); // Direction
	const Vec3 nudgeAxis = axis + Vec3(1.0f, 1.0f, 1.0f);
	const Vec3 radialVector = r * VMath::normalize(VMath::cross(axis, nudgeAxis));
	const float deltaHeight = 0.1f;
	const float deltaPhi = 360.0f / 45.0f;


	for (float thetaDeg = 0.0f; thetaDeg <= 360.0f; thetaDeg += deltaTheta)
	{
		for (float heightDeg = 0.0f; heightDeg < cylinderLength; heightDeg += deltaHeight) {
			Vec3 circle = sphereCentrePosA + radialVector + axis * heightDeg;
			MATH::Matrix3 rotationMatrix;
			rotationMatrix = MATH::MMath::rotate(thetaDeg, axis);
			circle = rotationMatrix * circle;
			vertices.push_back(circle);
			normals.push_back(circle);
		}
		
	}
	for (float thetaDeg = 0.0f; thetaDeg <= 90.0f; thetaDeg += deltaTheta)
	{
		Vec3 circle(r * sin(thetaDeg * DEGREES_TO_RADIANS), -r * cos(thetaDeg * DEGREES_TO_RADIANS) + sphereCentrePosA.y, 0.0f);
		for (float phiDeg = 0.0f; phiDeg <= 360.0f; phiDeg += deltaPhi) {
			MATH::Matrix3 rotationMatrix;
			//rotationMatrix = MATH::MMath::rotate(deltaPhi, axis);
			rotationMatrix = MATH::MMath::rotate(deltaPhi, Vec3(0.0f, 1.0f, 0.0f));
			//rotationMatrix = MATH::MMath::rotate(deltaPhi, Vec3(0.0f, 1.0f, 0.0f));
			circle = rotationMatrix * circle;
			vertices.push_back(circle);
			normals.push_back(circle - Vec3(sphereCentrePosA.x, sphereCentrePosA.y, sphereCentrePosA.z));
		}

		circle.set(r * sin(thetaDeg * DEGREES_TO_RADIANS), r * cos(thetaDeg * DEGREES_TO_RADIANS) + sphereCentrePosB.y, 0.0f);
		for (float phiDeg = 0.0f; phiDeg <= 360.0f; phiDeg += deltaPhi) {
			MATH::Matrix3 rotationMatrix;
			rotationMatrix = MATH::MMath::rotate(deltaPhi, axis);
			//rotationMatrix = MATH::MMath::rotate(deltaPhi, Vec3(0.0f, 1.0f, 0.0f));
			circle = rotationMatrix * circle;
			vertices.push_back(circle);
			normals.push_back(circle - Vec3(sphereCentrePosB.x, sphereCentrePosB.y, sphereCentrePosB.z));
		}
	}
}

RayIntersectionInfo GEOMETRY::Capsule::rayIntersectionInfo(const Ray& ray) const
{
	RayIntersectionInfo rayInfo;
	
	// Check A sphere
	rayInfo = checkEndSphere(sphereCentrePosA, ray);
	if (rayInfo.isIntersected) {
		std::cout << "Intersected with Sphere A at ";
		rayInfo.intersectionPoint.print();
		rayInfo = checkHalfSphere(rayInfo, sphereCentrePosA, sphereCentrePosB);
		if (rayInfo.isIntersected) {
			std::cout << "Half Sphere A intersection confirmed at ";
			rayInfo.intersectionPoint.print();
		}
	}
	// Check B sphere
	else {
		rayInfo = checkEndSphere(sphereCentrePosB, ray);
		if (rayInfo.isIntersected) {
			std::cout << "Intersected with Sphere B at ";
			rayInfo.intersectionPoint.print();
			rayInfo = checkHalfSphere(rayInfo, sphereCentrePosB, sphereCentrePosA);
			if (rayInfo.isIntersected) { 
				std::cout << "Half Sphere B intersection confirmed at "; 
				rayInfo.intersectionPoint.print(); 
			}
		}
	}

	if (!rayInfo.isIntersected) {
		std::cout << "Check Cylinder\n";
		rayInfo = checkCylinder(ray);
	}

	return rayInfo;
}

RayIntersectionInfo GEOMETRY::Capsule::checkEndSphere(MATH::Vec3 sphereCentre, const Ray& ray) const
{
	RayIntersectionInfo rayInfo;
	const Vec3 sphereToRayStart = ray.start - Vec3(sphereCentre.x, sphereCentre.y, sphereCentre.z);

	//Solve the quadratic equation for this inyersection
	//REFERENCE: Chapter 5 of Real Time Collision Detection by Ericson
	const float a = VMath::dot(ray.dir, ray.dir);
	const float b = 2.0f * VMath::dot(sphereToRayStart, ray.dir);
	const float c = VMath::dot(sphereToRayStart, sphereToRayStart) - r * r;
	QuadraticSolution soln = solveQuadratic(a, b, c);
	if (soln.numSolutions == NumSolutions::zeroRoots) {
		rayInfo.isIntersected = false;
	}
	else if (soln.secondRoot < 0.0f) {
		rayInfo.isIntersected = true;
		// ray is going backwards
		// second root is the smallest absolute value
		rayInfo.intersectionPoint = ray.start + ray.dir * soln.secondRoot;
		rayInfo.t = soln.secondRoot;
	}
	else {
		rayInfo.isIntersected = true;
		rayInfo.intersectionPoint = ray.start + ray.dir * soln.firstRoot;
		rayInfo.t = soln.firstRoot;
	}


	return rayInfo;
}

RayIntersectionInfo GEOMETRY::Capsule::checkHalfSphere(RayIntersectionInfo& rayInfoFullSphere, const MATH::Vec3& sphereCentre,
																	const MATH::Vec3& oppositeSphereCentre) const
{
	Vec3 CP = rayInfoFullSphere.intersectionPoint - sphereCentre;
	Vec3 CO = oppositeSphereCentre - sphereCentre;

	float dot_CP_CO = VMath::dot(CP, CO);

	if (dot_CP_CO < 0)
	{
		rayInfoFullSphere.isIntersected = true;
		return rayInfoFullSphere;
	}
	rayInfoFullSphere.isIntersected = false;
	return rayInfoFullSphere;
}

RayIntersectionInfo GEOMETRY::Capsule::checkCylinder(const Ray& ray) const
{
	RayIntersectionInfo rayInfo;
	rayInfo = checkInfiniteCylinder(ray);

	if (rayInfo.isIntersected == false) {
		// return it now as there are no roots or we are going backwards to hit a cylinder somehow
		return rayInfo;
	}

	// CALCULATE ab, ap, as = start-A
	Vec3 ab = sphereCentrePosB - sphereCentrePosA;
	Vec3 as = ray.start - sphereCentrePosA;

	//Vec3 ap = rayInfo.intersectionPoint - capCentrePosA;
	Vec3 ap = as + rayInfo.t * ray.dir;

	float dot_ap_ab = VMath::dot(ap, ab);
	float dot_as_ab = VMath::dot(as, ab);
	float dot_v_ab = VMath::dot(ray.dir, ab);


	Vec3 bs = ray.start - sphereCentrePosB;
	Vec3 ba = sphereCentrePosA - sphereCentrePosB;

	float dot_ab_ab = VMath::dot(ab, ab);
	float dot_bs_ba = VMath::dot(bs, ba);
	float dot_v_ba = VMath::dot(ray.dir, ba);


	if (dot_ap_ab < 0.0f) {
		// we are outside of cap centre A's plane (on the left side)
		if (VMath::dot(ray.dir, ab) > 0.0f) {
			// need to check for ray coming at endcap A. I wrote out this eqn on paper
			float t = -dot_as_ab / dot_v_ab;
			return checkEndCap(ray, t, sphereCentrePosA);
		}
		else {
			// ray going wrong way
			//no intersection
			rayInfo.isIntersected = false;
			return rayInfo;
		}
	}
	else if (dot_ap_ab > dot_ab_ab) {
		// We are outside of cap centre B's plane (on the right side)
		// If statement means the point is further than the length of the cylinder from endcap A
		if (VMath::dot(ray.dir, ab) < 0.0f) {
			std::cout << "B Cap\n";
			// ray is coming for endcap B
			float t = -dot_bs_ba / dot_v_ba;
			return checkEndCap(ray, t, sphereCentrePosB);
		}
		else {
			// ray going wrong way
				// .... = false
			rayInfo.isIntersected = false;
			return rayInfo;
		}
	}
	return rayInfo;
}

RayIntersectionInfo GEOMETRY::Capsule::checkInfiniteCylinder(const Ray& ray) const
{
	RayIntersectionInfo rayInfo;

	Vec3 ab = sphereCentrePosB - sphereCentrePosA;
	Vec3 abNorm = VMath::normalize(ab);
	Vec3 as = ray.start - sphereCentrePosA;


	const float a = VMath::dot(ray.dir, ray.dir) -
		VMath::dot(ray.dir, abNorm) * VMath::dot(ray.dir, abNorm);
	const float b = 2.0f * (VMath::dot(as, ray.dir) -
		VMath::dot(ray.dir, abNorm) * VMath::dot(as, abNorm));
	const float c = VMath::dot(as, as) -
		VMath::dot(as, abNorm) * VMath::dot(as, abNorm) - r * r;

	QuadraticSolution soln = solveQuadratic(a, b, c);

	if (soln.numSolutions == NumSolutions::zeroRoots) {
		rayInfo.isIntersected = false;
	}
	else if (soln.secondRoot < 0.0f) {
		rayInfo.isIntersected = true;
		// ray is going backwards
		// second root is the smallest absolute value
		rayInfo.intersectionPoint = ray.start + ray.dir * soln.secondRoot;
		rayInfo.t = soln.secondRoot;
	}
	else {
		rayInfo.isIntersected = true;
		rayInfo.intersectionPoint = ray.start + ray.dir * soln.firstRoot;
		rayInfo.t = soln.firstRoot;
	}

	return rayInfo;
}

RayIntersectionInfo GEOMETRY::Capsule::checkEndCap(const Ray& ray, float t, Vec3 endCap) const
{
	RayIntersectionInfo rayInfo;

	Vec3 q = ray.currentPosition(t);

	float magnitude = VMath::mag(q - endCap);

	if (magnitude <= r) {
		rayInfo.isIntersected = true;
		rayInfo.intersectionPoint = q;
	}
	else {
		rayInfo.isIntersected = false;
	}
	return rayInfo;
}
