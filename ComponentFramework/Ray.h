#ifndef RAY_H
#define RAY_H

#include "Vector.h"
#include "VMath.h"


using namespace MATH;

namespace GEOMETRY
{
	struct RayIntersectionInfo
	{
		bool isIntersected = false;
		Vec3 intersectionPoint;
		float t = 0.0f;

		void print()
		{
			if (isIntersected)
			{
				printf("Ray intersection with t value: %.2f\n", t);
			}
			else
			{
				printf("No intersection\n");
			}
		}
	};
	class Ray
	{
		public:

		Vec3 start;
		Vec3 dir;

		Vec3 currentPosition (float t) const;

		Ray(Vec3 start_, Vec3 dir_);
	};

}

#endif