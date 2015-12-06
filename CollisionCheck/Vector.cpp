#include "Vector.h"

void vec4::MultMatrix(const float * m)
{
	float v[4];
	for (int i = 0; i < 4; ++i)
	{
		v[i] = m[i] * x + m[4 + i] * y + m[8 + i] * z + m[12 + i] * w;
	}
}