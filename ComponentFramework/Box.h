#ifndef BOX_H
#define BOX_H

#include "Shape.h"
#include "Vector.h"
#include "Quaternion.h"
#include <vector>

namespace GEOMETRY {
	struct Slab {
		MATH::Vec3 normal;
		float distNear; // Signed distance from origin for near plane 
		float distFar; // Signed distance from origin for far plane 
	};
	struct Box : public Shape
	{
		// Represent an Orientated Bounding Box by centre position, half extents, and orientation.
		// REFERENCE: Real Time Collision Detection by Ericson
		// Ericson recommends keeping the orientation as a matrix for fast collision detection
		//,but I love quaternions too much
		MATH::Vec3 centre;
		MATH::Vec3 halfExtents;
		MATH::Quaternion orientation;

		// Default constructor sets the half extents along x, y, and z
		//to something other than zero, generates the vertices and normal 
		//for the shape, and stores the mesh data in order to render
		Box(); 
		Box(MATH::Vec3 centre_, MATH::Vec3 halfExtents_, MATH::Quaternion orientation_);
		void set(MATH::Vec3 centre_, MATH::Vec3 halfExtents_, MATH::Quaternion orientation_);
		// Fill the vertices and normals list with Vec3's to represent a box
		void generateVerticesAndNormals() override;
		RayIntersectionInfo rayIntersectionInfo(const Ray& ray) const override;


	};
}

#endif

