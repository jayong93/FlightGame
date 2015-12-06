#pragma once
#include <math.h>

struct vec3 {
	float x;
	float y;
	float z;

	vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) { }

	//operator vec4() { return vec4(x, y, z, 1); }
};

struct vec4
{
	float x, y, z, w;

	vec4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f) : x(x), y(y), z(z), w(w) {}

	operator vec3() { return vec3(x / w, y / w, z / w); }
	void MultMatrix(const float* m);
};