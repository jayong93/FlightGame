#pragma once
#include "Object.h"

class Unit : public Object
{
public:
	Unit(vec3& pos, float rad, float p, float y, float r);

protected:
	vec3 velocity;
	bool isAlly, isAlive;
};