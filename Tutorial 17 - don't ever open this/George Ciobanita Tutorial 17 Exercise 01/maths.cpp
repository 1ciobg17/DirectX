#include "maths.h"
#include <math.h>

/*Calculate the Dot product between two vectors*/
float Maths::Dot(const xyz* v1,const xyz* v2)
{
	float dot;
	dot = (v1->x*v2->x) + (v1->y*v2->y) + (v1->z*v2->z);
	return dot;
}

/*Calculate the Cross product between two vectors*/
Maths::xyz Maths::Cross(const xyz* v1,const xyz* v2)
{
	xyz v3;
	v3.x = (v1->y*v2->z) - (v1->z*v2->y);
	v3.y = (v1->z*v2->x) - (v1->x*v2->z);
	v3.z = (v1->x*v2->y) - (v1->y-v2->x);
	return v3;
}

/*Calculate the Normal to a plane in which a flat polygon lies*/
Maths::xyz Maths::Normal(const xyz* v1,const xyz* v2,const xyz* v3)
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

/*Calculate the Plane Equation of a plane in which a polygon lies*/
Maths::Plane Maths::TrianglePlane(const xyz* v1, const xyz* v2, const xyz* v3)
{
	xyz planeNormal = Normal(v1, v2, v3);
	float d = -Dot(&planeNormal, v1);
	Plane result;
	result.d = d;
	result.normal = planeNormal;
	return result;
}

/*
This function determined is the test point 'v' is:
1. behind the plan       (return value is negative)
2. on the plane          (return value is 0)
3. in front of the plane ( return value is positive)
*/
float Maths::PointPlane(const Plane& plane, const xyz& v)
{
	float result = Dot(&v, &plane.normal) + plane.d;
	return result;
}

/*Calculate the plane intersection between two points*/
/*
Gets the start and end point of a ray and calculates
the point of the ray that intersects
r=ray direction
normal=plane normal
point1=ray start
point2=ray end, ray start+length
point=point along the way
t=fraction between 0 and 1 that describes the position of the point along the ray
*/
Maths::xyz Maths::PlaneIntersection(const Maths::Plane* p, const Maths::xyz* point1, const Maths::xyz* point2)
{
	xyz r;
	r.x = point2->x - point1->x;
	r.y = point2->y - point1->y;
	r.z = point2->z - point1->z;
	float d=-Dot(&p->normal, point1);
	float t = (-d - Dot(&p->normal, point1)) / Dot(&p->normal, &r);
	xyz point;
	point.x = point1->x + (r.x*t);
	point.y = point1->y + (r.y*t);
	point.z = point1->z + (r.z*t);
	return point;
}

//This Function returns true if "point" is in a triangle(part of a plane, =>point is in a plane)
//Calculate the cross products of the different vectors that can result from the point and the triangle points
//Calculate the dot product by taking 2 of the cross results and if all 3 tests are positive or equal to = then the point is in a triangle
bool Maths::InTriangle(const xyz* triangle0, const xyz* triangle1, const xyz* triangle2, const xyz* point)
{
	xyz cross1, cross2, cross3, cross4, cross5, cross6;
	cross1 = Cross(&Minus(triangle1, triangle2), &Minus(triangle1, point));
	cross2 = Cross(&Minus(triangle1,triangle2), &Minus(triangle1,triangle0));
	cross3 = Cross(&Minus(triangle0, triangle2), &Minus(triangle0, point));
	cross4 = Cross(&Minus(triangle0, triangle2), &Minus(triangle0, triangle1));
	cross5 = Cross(&Minus(triangle0, triangle1), &Minus(triangle0, point));
	cross6 = Cross(&Minus(triangle0, triangle1), &Minus(triangle0, triangle2));
	if (Dot(&cross1, &cross2) >= 0 && Dot(&cross3, &cross4) >= 0 && Dot(&cross5, &cross6) >= 0)
		return true;
	return false;

}

Maths::xyz Maths::Minus(const xyz* v1,const xyz* v2)
{
	xyz result;
	result.x = v2->x - v1->x;
	result.y = v2->y - v1->y;
	result.z = v2->z - v1->z;
	return result;
}