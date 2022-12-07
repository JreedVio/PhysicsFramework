#include "Sphere.h"
#include "MMath.h"
#include "Matrix.h"

using namespace MATH;
using namespace GEOMETRY;

RayIntersectionInfo GEOMETRY::Sphere::rayIntersectionInfo(const Ray& ray) const
{
	RayIntersectionInfo rayInfo;
	const Vec3 sphereToRayStart = ray.start - Vec3(x, y, z);

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

void Sphere::generateVerticesAndNormals()
{
	const float deltaTheta = 1.0f;
	const float deltaPhi = 360.0f / 15.0f;
	for (float thetaDeg = 0.0f; thetaDeg <= 360.0f; thetaDeg += deltaTheta) // Changes vertically / Draws horizontally
	{
		Vec3 circle(r * sin(thetaDeg * DEGREES_TO_RADIANS), r * cos(thetaDeg * DEGREES_TO_RADIANS), 0.0f);
		// Changes horizontally / Draws vertically
		for (float phiDeg = 0.0f; phiDeg <= 360.0f; phiDeg += deltaPhi) {
			MATH::Matrix3 rotationMatrix;
			rotationMatrix = MATH::MMath::rotate(deltaPhi, Vec3(0.0f, 1.0f, 0.0f));
			circle = rotationMatrix * circle;
			vertices.push_back(circle);
			normals.push_back(circle - Vec3(x, y, z));
		}
	}
}