#include "Box.h"
#include "MMath.h"
#include "QMath.h"
#include "Matrix.h"
#include <array>

using namespace MATH;
using namespace GEOMETRY;

GEOMETRY::Box::Box()
{
	centre.set(0.0f, 0.0f, 0.0f);
	halfExtents.set(1.0f, 1.0f, 1.0f);
	MATH::Quaternion orientation = MATH::QMath::angleAxisRotation(0.0f, Vec3(1.0f, 0.0f, 0.0f));
	generateVerticesAndNormals();
	StoreMeshData(GL_TRIANGLES);
}

GEOMETRY::Box::Box(MATH::Vec3 centre_, MATH::Vec3 halfExtents_, MATH::Quaternion orientation_)
{
	set(centre_, halfExtents_, orientation_);
}

void GEOMETRY::Box::set(MATH::Vec3 centre_, MATH::Vec3 halfExtents_, MATH::Quaternion orientation_)
{
	centre.set(centre_.x, centre_.y, centre_.z);
	halfExtents.set(halfExtents_.x, halfExtents_.y, halfExtents_.z);
	orientation = orientation_;
	generateVerticesAndNormals();
	StoreMeshData(GL_TRIANGLES);
}

void GEOMETRY::Box::generateVerticesAndNormals()
{
#pragma region uniqueVertices

	std::vector<Vec3> uniqueVertices;

	const int top_right_front = 0;
	uniqueVertices.push_back(orientation * (centre + MATH::Vec3(halfExtents.x, halfExtents.y, halfExtents.z)));

	const int top_left_front = 1;
	uniqueVertices.push_back(orientation * (centre + MATH::Vec3(-halfExtents.x, halfExtents.y, halfExtents.z)));

	const int top_right_back = 2;
	uniqueVertices.push_back(orientation * (centre + MATH::Vec3(halfExtents.x, halfExtents.y, -halfExtents.z)));

	const int top_left_back = 3;
	uniqueVertices.push_back(orientation * (centre + MATH::Vec3(-halfExtents.x, halfExtents.y, -halfExtents.z)));

	const int bottom_right_front = 4;
	uniqueVertices.push_back(orientation * (centre + MATH::Vec3(halfExtents.x, -halfExtents.y, halfExtents.z)));

	const int bottom_left_front = 5;
	uniqueVertices.push_back(orientation * (centre + MATH::Vec3(-halfExtents.x, -halfExtents.y, halfExtents.z)));

	const int bottom_right_back = 6;
	uniqueVertices.push_back(orientation * (centre + MATH::Vec3(halfExtents.x, -halfExtents.y, -halfExtents.z)));

	const int bottom_left_back = 7;
	uniqueVertices.push_back(orientation * (centre + MATH::Vec3(-halfExtents.x, -halfExtents.y, -halfExtents.z)));

#pragma endregion

#pragma region vertices

	//////////// Top face
	vertices.push_back(uniqueVertices[top_right_front]);
	vertices.push_back(uniqueVertices[top_right_back]);
	vertices.push_back(uniqueVertices[top_left_front]);

	vertices.push_back(uniqueVertices[top_left_front]);
	vertices.push_back(uniqueVertices[top_right_back]);
	vertices.push_back(uniqueVertices[top_left_back]);

	// Bottom face

	vertices.push_back(uniqueVertices[bottom_right_back]);
	vertices.push_back(uniqueVertices[bottom_left_back]);
	vertices.push_back(uniqueVertices[bottom_right_front]);

	vertices.push_back(uniqueVertices[bottom_right_front]);
	vertices.push_back(uniqueVertices[bottom_left_back]);
	vertices.push_back(uniqueVertices[bottom_left_front]);

	/////////// Front face

	vertices.push_back(uniqueVertices[bottom_left_front]);
	vertices.push_back(uniqueVertices[top_left_front]);
	vertices.push_back(uniqueVertices[bottom_right_front]);

	vertices.push_back(uniqueVertices[bottom_right_front]);
	vertices.push_back(uniqueVertices[top_left_front]);
	vertices.push_back(uniqueVertices[top_right_front]);

	// Back face

	vertices.push_back(uniqueVertices[bottom_left_back]);
	vertices.push_back(uniqueVertices[top_left_back]);
	vertices.push_back(uniqueVertices[bottom_right_back]);

	vertices.push_back(uniqueVertices[bottom_right_back]);
	vertices.push_back(uniqueVertices[top_left_back]);
	vertices.push_back(uniqueVertices[top_right_back]);

	///////////// Right face

	vertices.push_back(uniqueVertices[bottom_right_back]);
	vertices.push_back(uniqueVertices[top_right_back]);
	vertices.push_back(uniqueVertices[bottom_right_front]);

	vertices.push_back(uniqueVertices[bottom_right_front]);
	vertices.push_back(uniqueVertices[top_right_back]);
	vertices.push_back(uniqueVertices[top_right_front]);

	// Left face

	vertices.push_back(uniqueVertices[bottom_left_back]);
	vertices.push_back(uniqueVertices[top_left_back]);
	vertices.push_back(uniqueVertices[bottom_left_front]);

	vertices.push_back(uniqueVertices[bottom_left_front]);
	vertices.push_back(uniqueVertices[top_left_back]);
	vertices.push_back(uniqueVertices[top_left_front]);

#pragma endregion

#pragma region normals
	// Top
	for (int i = 0; i < 6; i++)
		normals.push_back(Vec3(0.0f, 1.0f, 0.0f));

	// Bottom
	for (int i = 0; i < 6; i++)
		normals.push_back(Vec3(0.0f, -1.0f, 0.0f));

	// Front
	for (int i = 0; i < 6; i++)
		normals.push_back(Vec3(0.0f, 0.0f, 1.0f));

	// Back
	for (int i = 0; i < 6; i++)
		normals.push_back(Vec3(0.0f, 0.0f, -1.0f));

	// Right
	for (int i = 0; i < 6; i++)
		normals.push_back(Vec3(1.0f, 0.0f, 0.0f));

	// Left
	for (int i = 0; i < 6; i++)
		normals.push_back(Vec3(-1.0f, 0.0f, 0.0f));
#pragma endregion
}

RayIntersectionInfo GEOMETRY::Box::rayIntersectionInfo(const Ray& ray) const
{
	// Imagine a box is just made up of three infinite slabs
	Slab slabX;
	slabX.normal = Vec3(1.0f, 0.0f, 0.0f);
	slabX.distNear = -halfExtents.x;
	slabX.distFar = halfExtents.x;

	Slab slabY;
	slabY.normal = Vec3(0.0f, 1.0f, 0.0f);
	slabY.distNear = -halfExtents.y;
	slabY.distFar = halfExtents.y;

	//............

	Slab slabZ;
	slabZ.normal = Vec3(0.0f, 0.0f, 1.0f);
	slabZ.distNear = -halfExtents.z;
	slabZ.distFar = halfExtents.z;
	//................

	std::array<Slab, 3> slabs = {
		Slab{Vec3(1.0f, 0.0f, 0.0f), -halfExtents.x, halfExtents.x},
		Slab{Vec3(0.0f, 1.0f, 0.0f), -halfExtents.y, halfExtents.y},
		Slab{Vec3(0.0f, 0.0f, 1.0f), -halfExtents.z, halfExtents.z} };


	float tMin = 0.0f;
	float tMax = FLT_MAX;

	RayIntersectionInfo rayInfo;
	for (int i = 0; i < slabs.size(); i++) {
		float t1 = (slabs[i].distNear - ray.start[i]) / ray.dir[i];
		float t2 = (slabs[i].distFar - ray.start[i]) / ray.dir[i];
		//float t2 = (slabs[i].distFar ............;

		if (t1 > t2) { std::swap(t1, t2); }
		tMin = std::max(tMin, t1);
		tMax = std::min(tMax, t2);

		if (tMin > tMax) return rayInfo;
	}
	rayInfo.isIntersected = true;
	rayInfo.t = tMin;
	rayInfo.intersectionPoint = ray.currentPosition(tMin);
	return rayInfo;
}
