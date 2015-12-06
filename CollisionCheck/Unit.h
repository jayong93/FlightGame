#pragma once
#include "Object.h"
#include "Arm.h"
#include <vector>

class Unit : public Object
{
protected:
	vec3 velocity;
	bool isAlly, isAlive;
	std::vector<CubeObject*> cubeList;

	Arm arm;
public:
	Unit(vec3& pos, float rad, float p, float y, float r);

	void Move(const vec3 & );

	void Rotate(float, float, float);


};

class Drone : public Unit {
public:
	Drone(vec3 & pos, float rad = 5.0f, float p = 0.0f, float y = 0.0f, float r = 0.0f);

	void Render();

	void Update();
	
};