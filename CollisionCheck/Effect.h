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

class Spark : public Effect {
	vec3 position;
	vec3 direction;
	vec3 color;
	float fspeed;
	float timer, time;

	float Point[5][2];
	float starSize;

public:
	Spark(const vec3& pos, vec3& dir, vec3& color, float time) : position(pos), direction(dir), color(color),
		fspeed(50.0f), timer(0.0f),time(time), starSize(1.0f) {
		alive = true;
		for (int i = 0; i < 5; ++i) {
			float fRad = ((18 + 72 * i) / 180.0)*3.14;
			Point[i][0] = starSize*cosf(fRad);
			Point[i][1] = starSize*sinf(fRad);
		}
	}

	virtual void Render();
	virtual void Update(float frameTime);

};