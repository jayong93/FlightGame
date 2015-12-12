#include "Arm.h"
#include "std.h"
#include "Unit.h"
#include "Player.h"
#include "StageManager.h"

void Arm::Shot(bool isAlly, float* mtx)
{
	BulletManager* bulletmgr = BulletManager::Instance();
	bulletmgr->AddBullet(Bullet(isAlly, vec3(0.25f, 0.25f, 1.0f), mtx));
}

void Arm::Shot(bool isAlly, float* mtx, const vec3& dir)
{
	BulletManager* bulletmgr = BulletManager::Instance();
	bulletmgr->AddBullet(Bullet(isAlly, vec3(0.25f, 0.25f, 1.0f), mtx, dir));
}

void Arm::Shot(bool isAlly, float* mtx, const vec3& dir, Unit* unit)
{
	BulletManager* bulletmgr = BulletManager::Instance();
	bulletmgr->AddBullet(Bullet(isAlly, vec3(0.25f, 0.25f, 1.0f), mtx, dir, unit));
}

void Arm::Render()
{
	glColor3f(0.5f, 0.5f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glScalef(1.0f, 1.0f, 4.0f);
	glutSolidCube(1.0f);
	glPopMatrix();
}

Bullet::Bullet(bool isAlly, vec3 & ext, float* mtx) :CubeObject::CubeObject(ext, vec3(mtx[12], mtx[13], mtx[14]), ext.GetSize(), 0.0f, 0.0f, 0.0f), isAlly(isAlly), speed(2.0f), speedRate(2.0f)
, timer(0), isAlive(true), target(NULL)
{
	for (int i = 0; i < 16; ++i) matrix[i] = mtx[i];

	velocity = vec3(matrix[8], matrix[9], matrix[10]);
	velocity.Normalize();
}

Bullet::Bullet(bool isAlly, vec3 & ext, float* mtx, const vec3& dir) : CubeObject::CubeObject(ext, vec3(mtx[12], mtx[13], mtx[14]), ext.GetSize(), 0.0f, 0.0f, 0.0f), isAlly(isAlly), speed(1000.0f), speedRate(2.0f)
, timer(0), isAlive(true), target(NULL)
{
	for (int i = 0; i < 16; ++i) matrix[i] = mtx[i];

	velocity = dir;
	velocity.Normalize();

	position = position + (velocity * 10);
}

Bullet::Bullet(bool isAlly, vec3 & ext, float* mtx, const vec3& dir, Unit* unit)
	:CubeObject::CubeObject(ext, vec3(mtx[12], mtx[13], mtx[14]), ext.GetSize(), 0.0f, 0.0f, 0.0f), isAlly(isAlly), speed(1000.0f), speedRate(2.0f)
	, timer(0), isAlive(true), target(unit)
{
	for (int i = 0; i < 16; ++i) matrix[i] = mtx[i];

	velocity = dir;
	velocity.Normalize();

	position = position + (velocity * 10);
}

void Bullet::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(matrix);
	glScalef(2 * extent.x, 2 * extent.y, 2 * extent.z);
	glColor3f(1.0f, 0.3f, 0.3f);
	glutSolidCube(1.0f);
	glPopMatrix();
}

void Bullet::SetAlive(bool alive)
{
	isAlive = alive;
}

void Bullet::Update(float frameTime)
{
	if (target) {
		vec3 d = target->GetPos() - position;
		float distance = d.GetSize();
		d.Normalize();
		if (distance > 30.0f && DotProduct(d, velocity)>0 && !target->GetIsStelth()) {
			velocity = target->GetPos() - position;
			velocity.Normalize();
			vec3 unit(0, 0, 1);
			vec3 yv(velocity.x, 0, velocity.z);
			vec3 xv(0, velocity.y, sqrt(velocity.x*velocity.x + velocity.z*velocity.z));

			yv.Normalize(); xv.Normalize();
			yaw = acosf(unit.Dot(yv)) / 3.14f * 180.0f;
			pitch = -acosf(unit.Dot(xv)) / 3.14f * 180.0f;
			if (velocity.x < 0)
			{
				yaw *= -1;
			}
			if (velocity.y < 0)
			{
				pitch *= -1;
			}
			this->Object::GenMatrix();
		}
	}
	position.x += velocity.x*speed*frameTime;
	position.y += velocity.y*speed*frameTime;
	position.z += velocity.z*speed*frameTime;
	matrix[12] = position.x; matrix[13] = position.y; matrix[14] = position.z;
	speed += speedRate;
	++timer;
	CollisionCheck_Building();
	if (timer > 600) isAlive = false;
}

void Bullet::CollisionCheck_Building()
{
	StageManager* stm = StageManager::Instance();
	std::vector<std::vector<Building>*> buildingList = stm->GetBuildingList(position.x, position.z, boundingRad);
	for (int n = 0; n < buildingList.size(); ++n) {

		for (int i = 0; i < buildingList[n]->size(); ++i) {
			if (CubeObject::CollisionCheck(&(*buildingList[n])[i])) isAlive = false;
		}
	}
}


BulletManager* BulletManager::instance = nullptr;

BulletManager::BulletManager() { }

BulletManager * BulletManager::Instance()
{
	if (!instance) instance = new BulletManager;
	return instance;
}

void BulletManager::AddBullet(Bullet & bullet)
{
	bulletList.push_back(bullet);
}

void BulletManager::Render()
{
	for (unsigned int i = 0; i < bulletList.size(); ++i) bulletList[i].Render();
}

void BulletManager::Update(float frameTime)
{
	for (unsigned int i = 0; i < bulletList.size(); ++i) bulletList[i].Update(frameTime);
	for (auto i = bulletList.begin(); i != bulletList.end(); ) {
		if (!i->GetIsAlive()) i = bulletList.erase(i);
		else ++i;
	}
}

void BulletManager::CollisionCheck(Player * player)
{
	for (unsigned int i = 0; i < bulletList.size(); ++i) {
		if (player->ColiisionCheck_Cube(&bulletList[i])) {
			std::cout << "Collison with bullet" << std::endl;
			bulletList[i].SetAlive(false);
		}
	}
	for (auto i = bulletList.begin(); i != bulletList.end(); ) {
		if (!i->GetIsAlive()) i = bulletList.erase(i);
		else ++i;
	}
}