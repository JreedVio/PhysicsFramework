#ifndef CYLINDER_H
#define CYLINDER_H

#include "Shape.h"
#include "Vector.h"
#include "QuadraticSolution.h"
#include <vector>

namespace GEOMETRY {
	struct Cylinder : public Shape {
		// Represent a cylinder by the two end cap centre positions and the radius.REFERENCE: p.195 of Real Time Collision Detection by Ericson
		float r = 1.0f;
		MATH::Vec3 capCentrePosA;
		MATH::Vec3 capCentrePosB;

		// Default constructor sets the centre position of sphere end
		//A to something other than zero, generates the vertices and normal for the shape, 
		//and stores the mesh data in order to render
		Cylinder(); 
		Cylinder(float r_, MATH::Vec3 capCentrePosA_, MATH::Vec3 capCentrePosB_); 
		void set(float r_, MATH::Vec3 capCentrePosA_, MATH::Vec3 capCentrePosB_);
		// Fill the vertices and normals list with Vec3's to represent a cylinder
		void generateVerticesAndNormals() override;
		RayIntersectionInfo rayIntersectionInfo(const Ray& ray) const override;
		RayIntersectionInfo checkInfiniteCylinder(const Ray& ray) const;
		RayIntersectionInfo checkEndCap(const Ray& ray, float t) const;
	};
}
#endif