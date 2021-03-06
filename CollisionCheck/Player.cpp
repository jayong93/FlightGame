#include "Player.h"
#include "std.h"
#include "InputManager.h"
#include "Ring.h"
#include "StageManager.h"
#include <fmod.hpp>

using namespace FMOD;

extern System* fSystem;
extern Sound* sound[6];
extern Channel* channel;
extern Channel* bgmChan;

Player::Player(float x, float y, float z) : Unit(vec3(x, y, z), 30, 0, 0, 0), direction(0, 0, -1), isBoost(false), boostTimer(-1), stelthTimer(-1), alpha(1), mana(100), deathTimer(-1), hp(200)
{
	cubeList.push_back(new PlayerBody(position, vec3(0, 0, -2)));
	vec3 pos = position;

	cubeList.push_back(new PlayerWing(pos, vec3(0, 2.5, 5)));

	cubeList.push_back(new PlayerWing(pos, vec3(-4, 0, 5)));

	cubeList.push_back(new PlayerWing(pos, vec3(4, 0, 5)));

	velocity = vec3(0, 0, 0);
}

bool Player::ColiisionCheck(const Object* obj) const
{
	if (Object::CollisionCheck(obj))
	{
		for (auto& c : cubeList)
		{
			if (obj->CollisionCheck(c))
				return true;
		}
	}
	return false;
}

bool Player::ColiisionCheck_Cube(const CubeObject* obj)
{
	if (isAlive)
	{
		for (auto& c : cubeList)
		{
			if (obj->CollisionCheck(c))
			{
				hp -= 1;
				if (hp <= 0)
				{
					hp = 0;
					ProcessPlayerDeath();
				}
				return true;
			}
		}
	}
	return false;
}

bool Player::ColiisionCheck_Ring(const Ring* ring)
{
	if (Object::CollisionCheck(ring))
	{
		for (auto& c : cubeList)
		{
			if (ring->ItemCollisionCheck(c))
			{
				hp += 5;
				if (hp > 200)
					hp = 200;
				
				fSystem->playSound(sound[2], 0, false, &channel);
			}
			if (ring->CollisionCheck(c)) {
				ProcessPlayerDeath();
				return true;
			}
		}
	}
	return false;
}

bool Player::CollisionCheck_Building()
{
	StageManager* stm = StageManager::Instance();
	std::vector<std::vector<Building>*> buildingList = stm->GetBuildingList(position.x, position.z, boundingRad);
	for (int n = 0; n < buildingList.size(); ++n) {
		for (int i = 0; i < buildingList[n]->size(); ++i) {
			if (Object::CollisionCheck(&(*buildingList[n])[i]))
			{
				for (auto& c : cubeList)
				{
					if (c->CubeObject::CollisionCheck(&(*buildingList[n])[i])) {
						ProcessPlayerDeath();
						std::cout << "BuildingCollisionCheck" << std::endl;
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool Player::CollisionCheck_Drone(Drone * drone)
{
	if (Object::CollisionCheck(drone))
	{
		for (auto& c : cubeList)
		{
			if (drone->CollisionCheck(c)) {
				ProcessPlayerDeath();
				return true;
			}
		}
	}
	return false;
}

void Player::ProcessPlayerDeath()
{
	StageManager* stm = StageManager::Instance();
	stm->CallEffenct(EFFECT::FLAME, position, vec3((rand() % 9) / 10.0f + 0.1f, (rand() % 9) / 10.0f + 0.1f, (rand() % 9) / 10.0f + 0.1f));
	isAlive = false;
	deathTimer = 0;
	fSystem->playSound(sound[1], 0, false, &channel);
}

void Player::Render()
{
	if (isAlive) {
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			for (auto& c : cubeList)
			{
				c->Render();
			}
			glDisable(GL_BLEND);
		}
		glPopMatrix();
	}
}

void Player::Move(const vec3 & dir)
{
	position = position + dir;

	float mat[16], tmp[16];
	glMatrixMode(GL_MODELVIEW);
	glGetFloatv(GL_MODELVIEW_MATRIX, tmp);
	this->GetMatrix(mat);
	glLoadIdentity();
	glTranslatef(dir.x, dir.y, dir.z);
	glMultMatrixf(mat);
	glGetFloatv(GL_MODELVIEW_MATRIX, mat);
	this->SetMatrix(mat);
	glLoadMatrixf(tmp);

	for (auto& c : cubeList)
	{
		// 충돌체크를 위해 꼭 해줘야 함
		c->UpdateMatrix(matrix);
	}
}

void Player::Rotate(float pitch, float yaw, float roll)
{
	float mat[16], tmp[16];
	glMatrixMode(GL_MODELVIEW);
	glGetFloatv(GL_MODELVIEW_MATRIX, tmp);
	this->GetMatrix(mat);
	glLoadMatrixf(mat);
	glRotatef(pitch, 1, 0, 0);
	glRotatef(yaw, 0, 1, 0);
	glRotatef(roll, 0, 0, 1);
	glGetFloatv(GL_MODELVIEW_MATRIX, mat);
	this->SetMatrix(mat);
	glLoadMatrixf(tmp);

	for (auto& c : cubeList)
	{
		// 충돌체크를 위해 꼭 해줘야 함
		c->UpdateMatrix(matrix);
	}

	float speed = velocity.GetSize();
	vec3 unit(0, 0, -1);
	direction = unit.MultMatrix(mat);
	velocity = direction*speed;
}

void Player::Update(float frameTime)
{
	IM* im = IM::GetInstance();

	if (isAlive)
	{
		// 회전
		if (im->GetSpecialKeyState(GLUT_KEY_LEFT) == im->GetSpecialKeyState(GLUT_KEY_RIGHT));
		else if (im->GetSpecialKeyState(GLUT_KEY_LEFT))
		{
			this->Rotate(0, 0, 60 * frameTime);
		}
		else if (im->GetSpecialKeyState(GLUT_KEY_RIGHT))
		{
			this->Rotate(0, 0, -60 * frameTime);
		}
		if (im->GetSpecialKeyState(GLUT_KEY_UP) == im->GetSpecialKeyState(GLUT_KEY_DOWN));
		else if (im->GetSpecialKeyState(GLUT_KEY_UP))
		{
			float speed = velocity.GetSize();
			if (speed > 0)
				this->Rotate((-10 + -30 / 250.0 * speed) * frameTime, 0, 0);
		}
		else if (im->GetSpecialKeyState(GLUT_KEY_DOWN))
		{
			float speed = velocity.GetSize();
			if (speed > 0)
				this->Rotate((10 + 30 / 250.0 * speed) * frameTime, 0, 0);
		}
		if (im->GetKeyState('a') == im->GetKeyState('d'));
		else if (im->GetKeyState('a'))
		{
			this->Rotate(0, 20 * frameTime, 0);
		}
		else if (im->GetKeyState('d'))
		{
			this->Rotate(0, -20 * frameTime, 0);
		}

		// 가속, 감속, 부스터
		if (im->GetKeyState('q') && boostTimer < 0)
		{
			isBoost = true;
		}
		else if (im->GetKeyState('w'))
		{
			float inc = 40 * frameTime;
			float speed = velocity.GetSize();
			float limit = 250;
			if (speed < limit)
			{
				if (speed + inc > limit)
					velocity = direction * limit;
				else
					velocity = direction * (speed + inc);
			}
		}
		else if (im->GetKeyState('s'))
		{
			isBoost = false;
			float inc = -40 * frameTime;
			float speed = velocity.GetSize();
			float limit = 30;
			if (speed > limit)
			{
				if (speed + inc < limit)
					velocity = direction * limit;
				else
					velocity = direction * (speed + inc);
			}
		}

		// 부스터 적용
		if (isBoost)
		{
			if (boostTimer < 0)
				boostTimer = 0;
			else
				boostTimer += frameTime;
			float inc = 160 * frameTime;
			float limit = 350;
			float speed = velocity.GetSize();
			if (boostTimer >= 3)
			{
				isBoost = false;
			}
			else if (speed + inc < limit)
			{
				velocity = direction * (speed + inc);
			}
			else if (speed + inc > limit)
			{
				velocity = direction * limit;
			}
		}
		else if (boostTimer >= 0)
		{
			boostTimer += frameTime;
			float inc = -80 * frameTime;
			float speed = velocity.GetSize();
			float limit = 250;

			if (speed > 250.0f)
			{
				if (speed + inc < 250.0f)
					velocity = direction * 250.0f;
				else
					velocity = direction * (speed + inc);
			}
			if (boostTimer >= 20)
			{
				boostTimer = -1;
			}
		}

		// 스텔스 적용
		if (isStelth)
		{
			mana -= 10 * frameTime;
			if (mana <= 0)
			{
				mana = 0;
				isStelth = false;
				stelthTimer = 0;
				fSystem->playSound(sound[5], 0, false, &channel);
			}
		}
		// 마나 자동 충전
		else if (mana < 200)
		{
			mana += 5 * frameTime;
			if (mana > 200)
				mana = 200;
		}

		if (stelthTimer >= 0)
		{
			float inc = 1.4 * frameTime;
			float limit;
			if (isStelth)
			{
				limit = 0.3;
				alpha -= inc;
				if (alpha < limit)
				{
					alpha = limit;
					stelthTimer = -1;
				}
				for (auto& c : cubeList)
				{
					c->SetAlpha(alpha);
				}
			}
			else
			{
				limit = 1.0;
				alpha += inc;
				if (alpha > limit)
				{
					alpha = limit;
					stelthTimer = -1;
				}
				for (auto& c : cubeList)
				{
					c->SetAlpha(alpha);
				}
			}
		}

		// 스텔스 On/Off
		if (im->GetKeyState('e') && stelthTimer < 0)
		{
			isStelth = !isStelth;
			if (isStelth)
			{
				fSystem->playSound(sound[4], 0, false, &channel);
			}
			else
			{
				fSystem->playSound(sound[5], 0, false, &channel);
			}
			im->ReleaseKey('e');
			stelthTimer = 0;
		}

		// 시야각 조절
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		float speed = velocity.GetSize();
		float* fov = GetFovValue();
		*fov = 60 + (40 / 420.0 * speed);
		glMatrixMode(GL_MODELVIEW);

		this->Move(velocity*frameTime);

		if (position.GetSize() >= 5990)
		{
			ProcessPlayerDeath();
		}

		if (position.y < 0)
		{
			ProcessPlayerDeath();
		}
	}
	else
	{
		deathTimer += frameTime;
		if (deathTimer >= 3)
		{
			StageState* ss = GetStageState();
			*ss = INTRO;
			float* fov = GetFovValue();
			*fov = 60;
			bgmChan->setPaused(true);
		}
	}
}

PlayerBody::PlayerBody(const vec3& oPos, const vec3& rPos) : CubeObject(vec3(2, 1.25, 7), oPos + rPos, 7, 0, 0, 0), relativePos(rPos)
{
}

void PlayerBody::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	{
		glMultMatrixf(matrix);
		glScalef(extent.x * 2, extent.y * 2, extent.z * 2);
		glColor4f(1, 1, 1, alpha);
		glutSolidCube(1);
	}
	glPopMatrix();
}

void PlayerBody::UpdateMatrix(float * pMatrix)
{
	float tmp[16];
	glMatrixMode(GL_MODELVIEW);
	glGetFloatv(GL_MODELVIEW_MATRIX, tmp);
	glLoadMatrixf(pMatrix);
	glTranslatef(relativePos.x, relativePos.y, relativePos.z);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	glLoadMatrixf(tmp);
}

PlayerWing::PlayerWing(const vec3 & oPos, const vec3& rPos) : CubeObject(vec3(2, 1.25, 4), oPos + rPos, 4, 0, 0, 0), relativePos(rPos)
{
}

void PlayerWing::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	{
		glMultMatrixf(matrix);
		glScalef(extent.x * 2, extent.y * 2, extent.z * 2);
		glColor4f(0, 0, 0.5, alpha);
		glutSolidCube(1);
	}
	glPopMatrix();
}

void PlayerWing::UpdateMatrix(float * pMatrix)
{
	float tmp[16];
	glMatrixMode(GL_MODELVIEW);
	glGetFloatv(GL_MODELVIEW_MATRIX, tmp);
	glLoadMatrixf(pMatrix);
	glTranslatef(relativePos.x, relativePos.y, relativePos.z);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	glLoadMatrixf(tmp);
}
