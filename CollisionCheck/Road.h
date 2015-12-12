#pragma once
#include "Object.h"

class Road : public CubeObject
{
public:
	Road(float x, float y, float z, float w, float h, float pitch = 0, float yaw = 0, float roll = 0);

	Road(const vec3& p1, const vec3& p2, float w);

	virtual void Render();

private:
	static int texture;
	static int tWidth, tHeight;
	float width, height;
};