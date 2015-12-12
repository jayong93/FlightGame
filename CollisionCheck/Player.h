#pragma once
#include "Unit.h"

class Ring;
class Building;

class Player : public Unit
{
public:
	Player(float x, float y, float z);

	virtual bool ColiisionCheck(const Object* obj) const;
	bool ColiisionCheck_Cube(const CubeObject * obj) const;
	virtual void Render();
	virtual void Move(const vec3 &);
	virtual void Rotate(float, float, float);
	virtual void Update(float frameTime);
	float GetHp() const { return hp; }
	float GetMana() const { return mana; }
	bool ColiisionCheck_Ring(const Ring * ring);
	bool CollisionCheck_Building();
	bool CollisionCheck_Drone(Drone* drone);
	void ProcessPlayerDeath();

private:
	vec3 direction;
	bool isBoost, isStelth;
	float boostTimer, stelthTimer, alpha, mana;
	float fireTimer;
	mutable float hp;
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