#pragma once
#include "Object.h"
#include <vector>

class Unit;
class Player;
class Building;

class Arm {
public:
	Arm() { }
	void Shot(bool, float*);
	void Shot(bool, float*, const vec3&);
	void Shot(bool, float*, const vec3&, Unit*);
	void Render();
};

class Bullet : public CubeObject {
	bool isAlly;
	vec3 velocity;
	float speed;
	float speedRate;

	int timer;
	bool isAlive;
	Unit* target;
public:
	Bullet(bool, vec3&, float*);

	Bullet(bool, vec3&, float*, const vec3&);

	Bullet(bool, vec3&, float*, const vec3&, Unit*);

	void Render();

	void Update(float frameTime);

	bool GetIsAlive() { return isAlive; }

	void CollisionCheck_Building();

};

class BulletManager {
private:
	static BulletManager* instance;
	std::vector<Bullet> bulletList;

	BulletManager();
public:
	static BulletManager* Instance();

	void AddBullet(Bullet& bullet);

	void Render();

	void Update(float frameTime);

	void CollisionCheck(Player* player);


	std::vector<Bullet>* GetBulletList() { return &bulletList; }
};