#pragma once
#include "Unit.h"

class Ring;

class Player : public Unit
{
public:
	Player(float x, float y, float z);

	virtual bool ColiisionCheck(const Object* obj) const;
	virtual void Render();
	virtual void Move(const vec3 &);
	virtual void Rotate(float, float, float);
<<<<<<< HEAD
	void Update(float frameTime);
=======
	virtual void Update(float frameTime);
	float GetHp() const { return hp; }
>>>>>>> refs/remotes/origin/JaeYong
	bool ColiisionCheck_Ring(const Ring * ring) const;

private:
	vec3 direction;
	bool isBoost, isStelth;
	float boostTimer, stelthTimer, alpha, mana, hp;
	float fireTimer;
};

class PlayerBody : public CubeObject
{
public:
	PlayerBody(const vec3& oPos, const vec3& rPos);

	virtual void Render();
	virtual void UpdateMatrix(float* pMatrix);
private:
	vec3 relativePos;
};

class PlayerWing : public CubeObject
{
public:
	PlayerWing(const vec3& oPos, const vec3& rPos);

	virtual void Render();
	virtual void UpdateMatrix(float* pMatrix);
private:
	vec3 relativePos;
};