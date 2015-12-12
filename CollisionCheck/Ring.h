#pragma once
#include "Object.h"
#include <vector>

class Ring : public Object
{
protected:
	float width, height, depth;
	std::vector<CubeObject*> cubeList;

	bool isRotate;

public:
	Ring(float x, float y, float z, float w, float h, float d, float angle = 0, bool rotate = false);

	virtual void Render();
	virtual void Update(float frameTime);
	virtual bool CollisionCheck(const Object* obj) const;
};

class RotateRing : public Ring
{
private:
	float rad, angle, ringMat[16];

public:
	RotateRing(float x, float y, float z, float w, float h, float d, float r);

	virtual void Render();
	virtual void Update(float frameTime);
};

class RingEdge : public CubeObject
{
public:
	RingEdge(vec3&, vec3&, vec3& rPos, float, float, float, float);

	virtual void Render();
	virtual void UpdateMatrix(float* pMatrix);

private:
	vec3 relativePos;
};

class Item : public CubeObject
{
public:
	Item(vec3& extent, vec3& pos, float rad, float p, float y, float r);

	virtual void Render();
};
