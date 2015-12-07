#pragma once
#include "Object.h"
#include <vector>

class Arm {
public:
	Arm() { }
	void Shot(bool, float*);
	void Shot(bool, float*, const vec3&);
	void Render();
};

class Bullet : public CubeObject {
	bool isAlly;
	vec3 velocity;
	float speed;
	float speedRate;

	int timer;
	bool isAlive;
public:
	Bullet(bool, vec3&, float*);

	Bullet(bool, vec3&, float*, const vec3&);

	void Render();

	void Update();

	bool GetIsAlive() { return isAlive; }
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

	void Update();
};