#pragma once

class Maths
{
public:
	struct xyz { float x, y, z; };

	struct Plane
	{
		xyz normal; /*the normal to the plane*/
		float d; /* the 'd' constant in the equation for this plane*/
	};

	static xyz Minus(const xyz* v1,const xyz* v2);
	static float Dot(const xyz* v1, const xyz* v2);
	static xyz Cross(const xyz* v1,const xyz* v2);
	static xyz Normal(const xyz* v1,const xyz* v2,const xyz* v3);
	static Plane TrianglePlane(const xyz* v1, const xyz* v2, const xyz* v3);
	static float PointPlane(const Plane& plane,const xyz& v);
	static xyz PlaneIntersection(const Plane* p,const xyz* point1,const xyz* point2);
	static bool InTriangle(const xyz* triangle0,const xyz* triangle1,const xyz* triangle2,const xyz* point);
};
	