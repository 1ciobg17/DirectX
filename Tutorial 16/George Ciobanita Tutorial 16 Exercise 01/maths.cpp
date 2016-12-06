#include "maths.h"
#include <math.h>

float Maths::Dot(xyz* v1,xyz* v2)
{
	float dot;
	dot = (v1->x*v2->x) + (v1->y*v2->y) + (v1->z*v2->z);
	return dot;
}

Maths::xyz Maths::Cross(xyz* v1, xyz* v2)
{
	xyz v3;
	v3.x = (v1->y*v2->z) - (v1->z*v2->y);
	v3.y = (v1->z*v2->x) - (v1->x*v2->z);
	v3.z = (v1->x*v2->y) - (v1->y-v2->x);
	return v3;
}

Maths::xyz Maths::Normal(xyz* v1, xyz* v2, xyz* v3)
{
	xyz first;
	first.x = v2->x - v1->x;
	first.y = v2->y - v1->y;
	first.z = v2->z - v1->z;
	xyz second;
	second.x = v3->x - v1->x;
	second.y = v3->y - v1->y;
	second.z = v3->z - v1->z;
	xyz cross = Cross(&first, &second);
	float length = sqrt((cross.x*cross.x)+(cross.y*cross.y)+(cross.z*cross.z));
	xyz normal;
	normal.x = cross.x / length;
	normal.y = cross.y / length;
	normal.z = cross.z / length;
	return normal;
}

Maths::Plane Maths::TrianglePlane(xyz* v1, xyz* v2, xyz* v3)
{
	xyz planeNormal = Normal(v1, v2, v3);
	float d = -Dot(&planeNormal, v1);
	Plane result;
	result.d = d;
	result.normal = planeNormal;
	return result;
}

float Maths::PointPlane(Plane plane, xyz v)
{
	float result = (plane.normal.x*v.x) + (plane.normal.y*v.y) + (plane.normal.z*v.z) + plane.d;
	return result;
}