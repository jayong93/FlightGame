#pragma once
#include <math.h>

struct vec4;

struct vec3 {
	float x;
	float y;
	float z;

	vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) { }

	operator vec4() const;

	void Normalize()
	{
		float div = float(sqrt(x*x + y*y + z*z));
		if (div == 0) return;
		x /= div, y /= div, z /= div;
	}

	float GetSize() const
	{
		return float(sqrt(x*x + y*y + z*z));
	}

	vec3 operator+(const vec3& v) const
	{
		return vec3(v.x + x, v.y + y, v.z + z);
	}

	vec3 operator*(float m) const
	{
		return vec3(x*m, y*m, z*m);
	}

	vec3 operator*(const vec3& v) const
	{
		return vec3(y*v.z - z*v.y, -(x*v.z - z*v.x), x*v.y - y*v.x);
	}

	vec3 operator/(float d) const
	{
		return vec3(x / d, y / d, z / d);
	}

	vec3 operator-(const vec3& v) const
	{
		return vec3(x - v.x, y - v.y, z - v.z);
	}

	vec3 operator-() const
	{
		return vec3(-x, -y, -z);
	}

	float Dot(const vec3& v) const
	{
		return v.x * x + v.y * y + v.z * z;
	}

	vec3 MultMatrix(const float* m) const;
};

struct vec4
{
	float x, y, z, w;

	vec4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f) : x(x), y(y), z(z), w(w) {}

	operator vec3() const;
	vec4 MultMatrix(const float* m) const;
};

struct Node {
	int row;
	int col;

	Node(int r = -1, int c = -1) : row(r), col(c) { }
};
