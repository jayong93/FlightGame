#pragma once
#include "Object.h"
#include "Arm.h"
#include <vector>

class State;
class Patrol;
class Trace;

class Unit : public Object
{
protected:
	vec3 velocity;
	bool isAlly, isAlive;
	std::vector<CubeObject*> cubeList;

	Arm arm;
public:
	Unit(vec3& pos, float rad, float p, float y, float r);

	virtual void Render() { }

	void Move(const vec3 &);

	void Rotate(float, float, float);

	virtual void CameraTransform() { }

	friend class Patrol;
	friend class Trace;
};

class Drone : public Unit {
	Node desNode;

	std::vector<Node> route;

	Unit* target;
	State* state;

	Node prevTargetnode;
	int croute;

	int shotTimer;
	bool polling;
public:
	Drone(Unit* target, vec3 & pos, float rad = 5.0f, float p = 0.0f, float y = 0.0f, float r = 0.0f);

	void SetDes();

	void SetDes(int row, int col);

	void Render();

	virtual void Update(float frameTime);

	void Shot();

	void ChangeState(State*);

	void SetDirection(vec3&);

	void Polling();

	friend class Patrol;
	friend class Trace;
};


class TempTarget : public Unit {
public:
	TempTarget(vec3 & pos, float rad = 5.0f, float p = 0.0f, float y = 0.0f, float r = 0.0f);

	virtual void Render();

	void CameraTransform();
};