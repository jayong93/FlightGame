#include "Vector.h"
#include "std.h"

vec3::operator vec4() const
{
	return vec4(x, y, z, 1);
}

vec3 vec3::MultMatrix(const float * m) const
{
	float v[3];
	for (int i = 0; i < 3; ++i)
	{
		v[i] = m[i] * x + m[4 + i] * y + m[8 + i] * z;
	}
	return vec3();
}

vec4::operator vec3() const
{
	return vec3(x / w, y / w, z / w);
}

vec4 vec4::MultMatrix(const float * m) const
{
	float v[4];
	for (int i = 0; i < 4; ++i)
	{
		v[i] = m[i] * x + m[4 + i] * y + m[8 + i] * z + m[12 + i] * w;
	}
	return vec4(v[0], v[1], v[2], v[3]);
}
