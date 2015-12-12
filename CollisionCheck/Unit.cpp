#include "Unit.h"
#include "std.h"
#include "text.h"
#include "StageManager.h"
#include "AIState.h"



Unit::Unit(vec3 & pos, float rad, float p, float y, float r) : Object(pos, rad, p, y, r), velocity(0, 0, 0), isAlly(false), isAlive(true), arm()
{
}


void Unit::Move(const vec3 & shift)
{
	this->Object::Move(shift);
	this->Object::GenMatrix();
	for (unsigned int i = 0; i < cubeList.size(); ++i) {
		cubeList[i]->Object::Move(shift);
		cubeList[i]->Object::GenMatrix();
	}

}

void Unit::Rotate(float pdel, float ydel, float rdel)
{
	this->Object::Rotate(pdel, ydel, rdel);
	this->Object::GenMatrix();
	for (unsigned int i = 0; i < cubeList.size(); ++i) {
		cubeList[i]->Object::Rotate(pdel, ydel, rdel);
		cubeList[i]->Object::GenMatrix();
	}
}


Drone::Drone(Unit* target, vec3 & pos, float rad, float p, float y, float r) : Unit(pos, rad, p, y, r),
target(target), shotTimer(0), polling(false)
{
	cubeList.push_back(new CubeObject(vec3(2.5f*sqrt(2), 2.5f * sqrt(2), 2.5f * sqrt(2)), pos, rad, p, y, r));
	cubeList.shrink_to_fit();
	state = Patrol::Instance();
}

void Drone::SetDes()
{
	StageManager* stm = StageManager::Instance();
	desNode = stm->GetNearestNode(position.x, position.z);
	velocity.x = (desNode.col*387.1f - 6000.0f) - position.x;
	velocity.y = 0.0f;
	velocity.z = (desNode.row*387.1f - 6000.0f) - position.z;
	velocity.Normalize();
	SetDirection(velocity);
}

void Drone::SetDes(int row, int col)
{
	desNode.row = row; desNode.col = col;
	velocity.x = (desNode.col*387.1f - 6000.0f) - position.x;
	velocity.y = 0.0f;
	velocity.z = (desNode.row*387.1f - 6000.0f) - position.z;
	velocity.Normalize();
}

void Drone::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); {
		glMultMatrixf(matrix);

		glColor3f(0.7f, 0.7f, 0.7f);
		glutSolidSphere(boundingRad, 32, 32);

		glPushMatrix(); {
			glTranslatef(0.0f, 0.0f, boundingRad - boundingRad / 2.0f);
			glColor3f(0.3f, 0.3f, 0.3f);
			glutSolidCube(boundingRad + 1.0f);


			glTranslatef(0.0f, 0.0f, boundingRad - boundingRad / 2.0f);
			glPushMatrix(); {
				glColor3f(0.8f, 0.8f, 0.8f);
				glTranslatef(0.0f, 0.0f, boundingRad / 30.0f);
				glScalef(3.0f, 3.0f, 3.0f);
				t3dDraw3D("A166", 0, 0, 0.2f);
			}glPopMatrix();
			glPushMatrix(); {
				glColor3f(0.3f, 0.3f, 1.0f);
				glTranslatef(boundingRad - boundingRad / 2.0f - (boundingRad / 10.0f)*2.2f, boundingRad - boundingRad / 2.0f - (boundingRad / 10.0f)*2.5f, 0.0f);
				glutSolidSphere(boundingRad / 10.0f, 32, 32);
			}glPopMatrix();
			glPushMatrix(); {
				glColor3f(0.3f, 1.0f, 0.3f);
				glTranslatef(-(boundingRad - boundingRad / 2.0f - (boundingRad / 10.0f)*2.2f), -(boundingRad - boundingRad / 2.0f - (boundingRad / 10.0f)*2.5f), 0.0f);
				glutSolidSphere(boundingRad / 10.0f, 32, 32);
			}glPopMatrix();

		}glPopMatrix();
		//Draw Arm
		glPushMatrix(); {
			glTranslatef(-boundingRad + 0.5f, 0.0f, 0.0f);
			glPushMatrix();
			glTranslatef(1.0f, 0.0f, boundingRad);
			arm.Render();
			glPopMatrix();
			glScalef(0.2f, 1.0f, 1.0f);
			glColor3f(0.6f, 0.6f, 0.6f);
			glutSolidSphere(boundingRad - 3.0f, 32, 32);
		}glPopMatrix();
		glPushMatrix(); {
			glTranslatef(boundingRad - 0.5f, 0.0f, 0.0f);
			glPushMatrix();
			glTranslatef(-1.0f, 0.0f, boundingRad);
			arm.Render();
			glPopMatrix();
			glScalef(0.2f, 1.0f, 1.0f);
			glColor3f(0.6f, 0.6f, 0.6f);
			glutSolidSphere(boundingRad - 3.0f, 32, 32);
		}glPopMatrix();
	}glPopMatrix();
}

void Drone::Shot() {
	vec3 dir = target->GetPos() - position;
	dir.Normalize();
	float bulletmtx[16];
	float tmpmtx[16];
	glMatrixMode(GL_MODELVIEW);
	glGetFloatv(GL_MODELVIEW_MATRIX, tmpmtx);
	glLoadIdentity();

	glLoadMatrixf(matrix);
	glTranslatef(-boundingRad + 3.0f, 0.0f, boundingRad);
	glGetFloatv(GL_MODELVIEW_MATRIX, bulletmtx);
	arm.Shot(false, bulletmtx, dir, target);

	glLoadIdentity();
	glLoadMatrixf(tmpmtx);

	glMatrixMode(GL_MODELVIEW);
	glGetFloatv(GL_MODELVIEW_MATRIX, tmpmtx);
	glLoadIdentity();

	glLoadMatrixf(matrix);
	glTranslatef(boundingRad - 3.0f, 0.0f, boundingRad);
	glGetFloatv(GL_MODELVIEW_MATRIX, bulletmtx);
	arm.Shot(false, bulletmtx, dir, target);

	glLoadIdentity();
	glLoadMatrixf(tmpmtx);
}

void Drone::Update(float frameTime)
{
	state->Update(*this, frameTime);

	this->Object::GenMatrix();
	for (unsigned int i = 0; i < cubeList.size(); ++i) {
		cubeList[i]->Object::GenMatrix();
	}
	shotTimer = (shotTimer + 1) % 5;
}

void Drone::ChangeState(State * st)
{
	state = st;
}

void Drone::Polling()
{
	StageManager* stm = StageManager::Instance();
	std::vector<std::vector<Building>*> buildingList = stm->GetBuildingList(position.x, position.z, boundingRad);
	bool flag = false;
	for (int n = 0; n < buildingList.size(); ++n) {
		for (int i = 0; i < buildingList[n]->size(); ++i) {
			float distance;
			if ((*buildingList[n])[i].CollisionCheckRay(position, vec3((desNode.col*387.1f - 6000.0f), position.y, (desNode.row*387.1f - 6000.0f)), distance)) {
				flag = true;
			}
		}
	}
	if (!flag) {
		velocity.x = (desNode.col*387.1f - 6000.0f) - position.x;
		velocity.y = 0.0f;
		velocity.z = (desNode.row*387.1f - 6000.0f) - position.z;
		velocity.Normalize();
		polling = false;
	}
}

bool Drone::CollisionCheck(const CubeObject * obj)
{

	for (auto& c : cubeList)
	{
		if (obj->CollisionCheck(c)) {
			std::cout << "CollisionDrone" << std::endl;
			StageManager* stm = StageManager::Instance();
			stm->CallEffenct(EFFECT::FLAME, position, vec3((rand() % 9) / 10.0f + 0.1f, (rand() % 9) / 10.0f + 0.1f, (rand() % 9) / 10.0f + 0.1f));
			isAlive = false;
			return true;
		}
	}
	return false;
}

void Drone::SetDirection(vec3 & v)
{
	//if (v.y != 0.0f) roll = (atan2f(v.y, v.x) / 3.14f)*180.0f;
	//if (abs(v.x) > 0.1f) yaw = (atan2f(v.x, v.z) / 3.14f)*180.0f;
	//if (abs(v.y) > 0.1f) pitch = -(atan2f(v.y, v.z) / 3.14f)*180.0f;
	vec3 unit(0, 0, 1);
	vec3 yv(v.x, 0, v.z);
	vec3 xv(0, v.y, sqrt(v.x*v.x + v.z*v.z));

	yv.Normalize(); xv.Normalize();
	yaw = acosf(unit.Dot(yv)) / 3.14f * 180.0f;
	pitch = -acosf(unit.Dot(xv)) / 3.14f * 180.0f;
	if (v.x < 0)
	{
		yaw *= -1;
	}
	if (v.y < 0)
	{
		pitch *= -1;
	}
	//printf("Pitch : %f, Yaw : %f\n", pitch, yaw);
	this->Object::GenMatrix();
}

TempTarget::TempTarget(vec3 & pos, float rad, float p, float y, float r) : Unit(pos, rad, p, y, r)
{
}

void TempTarget::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); {
		glMultMatrixf(matrix);

		glColor3f(1.0f, 1.0f, 1.0f);
		glutSolidSphere(10, 32, 32);

	}glPopMatrix();
}

void TempTarget::CameraTransform()
{
	glMatrixMode(GL_MODELVIEW);
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(-roll, 0.0f, 0.0f, 1.0f);
	glRotatef(-yaw, 0.0f, 1.0f, 0.0f);
	glRotatef(-pitch, 1.0f, 0.0f, 0.0f);
	glTranslatef(-position.x, -position.y, -position.z);
}

