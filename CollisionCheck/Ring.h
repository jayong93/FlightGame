#pragma once
#include "Object.h"
#include <vector>

class Item : public CubeObject
{
public:
	Item(vec3& extent, vec3& pos, float rad, float p, float y, float r);

	virtual void Render();

	virtual void Update(float frameTime);
};

class Ring : public Object
{
private:
	float width, height, depth;
	std::vector<CubeObject*> cubeList;
	Item item;
	bool isRotate;

public:
	Ring(float x, float y, float z, float w, float h, float d);

	virtual void Render();
	virtual void Update(float frameTime);
};

class RingEdge : public CubeObject
{
public:
	RingEdge(vec3&, vec3&, vec3& rPos, float, float, float, float);

	virtual void Render();

private:
	vec3 relativePos;
};