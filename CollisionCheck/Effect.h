#pragma once
#include "Vector.h"

class Effect{
protected:
	bool alive;
public:
	virtual void Render() = 0;
	virtual void Update(float frameTime) = 0;
	bool GetAlive() { return(alive); }
};

class Flame : public Effect {
	vec3 position;
	vec3 direction;
	vec3 color;
	float fspeed, fspeedRate;
	float fRaudius;
public:
	Flame(const vec3& pos, vec3& dir, vec3& color) : position(pos), direction(dir), color(color),
		fspeed(300.0f), fspeedRate(50.0f), fRaudius(2.0f) {
		alive = true;
	}

	virtual void Render();
	virtual void Update(float frameTime);


};