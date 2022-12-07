#include "Ray.h"


GEOMETRY::Ray::Ray(Vec3 start_, Vec3 dir_)
{

	 start = start_;
	 dir = dir_;

}

Vec3 GEOMETRY::Ray::currentPosition (float t) const
{
	
	Vec3 Pos = start + dir * t;
	//Vec3 Pos = start + t * dir;

	return Pos;
}
