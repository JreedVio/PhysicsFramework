#include "Cylinder.h"
#include "MMath.h"
#include "QMath.h"
#include "Matrix.h"

using namespace MATH;
using namespace GEOMETRY;

GEOMETRY::Cylinder::Cylinder()
{
	//capCentrePosA = MATH::Vec3(1,1,1);
	capCentrePosA.set(0.0f, 1.0f, 0.0f);
	capCentrePosB.set(0.0f, 1.0f, 10.0f);
	generateVerticesAndNormals();
	StoreMeshData(GL_POINTS);

}

GEOMETRY::Cylinder::Cylinder(float r_, MATH::Vec3 capCentrePosA_, MATH::Vec3 capCentrePosB_)
{
	set(r_, capCentrePosA_, capCentrePosB_);
}

void GEOMETRY::Cylinder::set(float r_, MATH::Vec3 capCentrePosA_, MATH::Vec3 capCentrePosB_)
{
	r = r_;
	capCentrePosA.set(capCentrePosA_.x, capCentrePosA_.y, capCentrePosA_.z);
	capCentrePosB.set(capCentrePosB_.x, capCentrePosB_.y, capCentrePosB_.z);
	generateVerticesAndNormals();
	StoreMeshData(GL_POINTS);
}

void GEOMETRY::Cylinder::generateVerticesAndNormals()
{
	const float deltaTheta = 1.0f;
	const float cylinderLength = VMath::mag(capCentrePosB - capCentrePosA);
	const Vec3 axis = VMath::normalize(capCentrePosB - capCentrePosA); // Direction
	const Vec3 nudgeAxis = axis + Vec3(1.0f, 1.0f, 1.0f);
	const Vec3 radialVector = r * VMath::normalize(VMath::cross(axis, nudgeAxis));	
	const float deltaHeight = 0.1f;
	const float deltaRad = 0.1f;

	for (float thetaDeg = 0.0f; thetaDeg <= 360.0f; thetaDeg += deltaTheta)
	{
		for (float heightDeg = 0.0f; heightDeg < cylinderLength; heightDeg += deltaHeight) {
			Vec3 circle = capCentrePosA + radialVector + axis * heightDeg;
			MATH::Matrix3 rotationMatrix;
			rotationMatrix = MATH::MMath::rotate(thetaDeg, axis);
			circle = rotationMatrix * circle;
			vertices.push_back(circle);
			normals.push_back(circle);
		}
		for (float radFactor = 0.0f; radFactor <= 1.0f; radFactor += deltaRad) {
			Vec3 circleA = capCentrePosA + radialVector * radFactor;
			Vec3 circleB = capCentrePosB + radialVector * radFactor;
			MATH::Matrix3 rotation = MMath::rotate(thetaDeg, axis);
			circleA = rotation * circleA;
			circleB = rotation * circleB;
			vertices.push_back(circleA);
			normals.push_back(circleA);
			vertices.push_back(circleB);
			normals.push_back(circleB);
		}
	}
	

}

RayIntersectionInfo GEOMETRY::Cylinder::rayIntersectionInfo(const Ray& ray) const
{
	// Check against an infinite cylinder
	RayIntersectionInfo rayInfo;
	rayInfo = checkInfiniteCylinder(ray);

	if (rayInfo.isIntersected == false) {
		// return it now as there are no roots or we are going backwards to hit a cylinder somehow
		return rayInfo;
	}

	// CALCULATE ab, ap, as = start-A
	Vec3 ab = capCentrePosB - capCentrePosA;
	Vec3 as = ray.start - capCentrePosA;

	//Vec3 ap = rayInfo.intersectionPoint - capCentrePosA;
	Vec3 ap = as + rayInfo.t * ray.dir;

	float dot_ap_ab = VMath::dot(ap, ab);
	float dot_as_ab = VMath::dot(as, ab);
	float dot_v_ab = VMath::dot(ray.dir, ab);


	Vec3 bs = ray.start - capCentrePosB;
	Vec3 ba = capCentrePosA - capCentrePosB;

	float dot_ab_ab = VMath::dot(ab, ab);
	float dot_bs_ba = VMath::dot(bs, ba);
	float dot_v_ba = VMath::dot(ray.dir, ba);


	if (dot_ap_ab < 0.0f) {
		// we are outside of cap centre A's plane (on the left side)
		if (VMath::dot(ray.dir, ab) > 0.0f) {
			// need to check for ray coming at endcap A. I wrote out this eqn on paper
			float t = -dot_as_ab / dot_v_ab;
			return checkEndCap(ray, t, capCentrePosA);
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
			return checkEndCap(ray, t, capCentrePosB);
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

RayIntersectionInfo GEOMETRY::Cylinder::checkInfiniteCylinder(const Ray& ray) const
{
	RayIntersectionInfo rayInfo;

	Vec3 ab = capCentrePosB - capCentrePosA;
	Vec3 abNorm = VMath::normalize(ab);
	Vec3 as = ray.start - capCentrePosA;


	const float a = VMath::dot(ray.dir, ray.dir) - VMath::dot(ray.dir, abNorm) * VMath::dot(ray.dir, abNorm);
	const float b = 2.0f * (VMath::dot(as, ray.dir) - VMath::dot(ray.dir, abNorm) * VMath::dot(as, abNorm));
	const float c = VMath::dot(as, as) - VMath::dot(as, abNorm) * VMath::dot(as, abNorm) - r * r;

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

RayIntersectionInfo GEOMETRY::Cylinder::checkEndCap(const Ray& ray, float t, Vec3 endCap) const
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
