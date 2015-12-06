#pragma once
#include "Unit.h"

class Player : public Unit
{
public:
	Player(float x, float y, float z);

	virtual bool ColiisionCheck(const Object& obj);
	virtual void Render();
	virtual void Move(const vec3 &);
	virtual void Rotate(float, float, float);
	void Update();
};

class PlayerBody : public CubeObject
{
public:
	PlayerBody(const vec3& oPos, const vec3& rPos);

	virtual void Render();

private:
	vec3 relativePos;
};

class PlayerWing : public CubeObject
{
public:
	PlayerWing(const vec3& oPos, const vec3& rPos);

	virtual void Render();

private:
	vec3 relativePos;
};