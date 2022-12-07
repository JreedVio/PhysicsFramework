#ifndef CAPSULE_H
#define CAPSULE_H

#include "Shape.h"
#include "Vector.h"
#include <vector>

namespace GEOMETRY {
	struct Capsule : public Shape
	{
	public:
		// A capsule is represented by two spheres swept along connecting axis. 
		// REFERENCE: Robust Contact Creation for Physics Simulations, D. Gregorius// 
		// http://media.steampowered.com/apps/valve/2015/DirkGregorius_Contacts.pdf
		float r = 1.0f;
		MATH::Vec3 sphereCentrePosA;
		MATH::Vec3 sphereCentrePosB;

		// Default constructor sets the centre position of cap A to something other than zero, generates the vertices and normal for the shape, and stores the mesh data in order to render
		Capsule();
		Capsule(float r_, MATH::Vec3 sphereCentrePosA_, MATH::Vec3 sphereCentrePosB_);
		void set(float r_, MATH::Vec3 sphereCentrePosA_, MATH::Vec3 sphereCentrePosB_);
		// Fill the vertices and normalslist with Vec3's 
		// to represent a capsule
		void generateVerticesAndNormals() override;
		RayIntersectionInfo rayIntersectionInfo(const Ray& ray) const override;
		RayIntersectionInfo checkEndSphere(MATH::Vec3 sphereCentre, const Ray& ray)	const;
		RayIntersectionInfo checkHalfSphere(const RayIntersectionInfo&
			rayInfoFullSphere, const MATH::Vec3& sphereCentre) const;
		RayIntersectionInfo checkCylinder(const Ray& ray) const;
		RayIntersectionInfo checkInfiniteCylinder(const Ray& ray) const;
		RayIntersectionInfo checkEndCap(const Ray& ray, float t) const;



	};
}
#endif

