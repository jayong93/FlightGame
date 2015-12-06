#include "Arm.h"
#include "std.h"


void Arm::Shot(bool isAlly, float* mtx)
{
	BulletManager* bulletmgr = BulletManager::Instance();
	bulletmgr->AddBullet(Bullet(isAlly, vec3(0.25f, 0.25f, 1.0f), mtx));
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

Bullet::Bullet(bool isAlly, vec3 & ext, float* mtx)
	:CubeObject::CubeObject(ext, vec3(mtx[12],mtx[13],mtx[14]),ext.GetSize(),0.0f, 0.0f, 0.0f), isAlly(isAlly), speed(2.0f), speedRate(2.0f)
	,timer(0), isAlive(true)
{
	for (int i = 0; i < 16; ++i) matrix[i] = mtx[i];

	velocity = vec3(matrix[8],matrix[9],matrix[10]);
	velocity.Normalize();
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

void Bullet::Update()
{
	position.x += velocity.x*speed;
	position.y += velocity.y*speed;
	position.z += velocity.z*speed;
	matrix[12] = position.x; matrix[13] = position.y; matrix[14] = position.z;
	speed += speedRate;
	++timer;
	if (timer > 600) isAlive = false;
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

void BulletManager::Update()
{
	for (unsigned int i = 0; i < bulletList.size(); ++i) bulletList[i].Update();
	for (auto i = bulletList.begin(); i != bulletList.end(); ) {
		if (!i->GetIsAlive()) i = bulletList.erase(i);
		else ++i;
	}
}
