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

	float Dot(xyz* v1, xyz* v2);
	xyz Cross(xyz* v1, xyz* v2);
	xyz Normal(xyz* v1, xyz* v2, xyz* v3);
	Plane TrianglePlane(xyz* v1, xyz* v2, xyz* v3);
	float PointPlane(Plane plane, xyz v);
};
	