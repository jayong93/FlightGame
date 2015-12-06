#include "Unit.h"
#include "std.h"
#include "text.h"



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


Drone::Drone(vec3 & pos, float rad, float p, float y, float r) : Unit(pos, rad, p, y, r)
{
	cubeList.push_back(new CubeObject(vec3(2.5f*sqrt(2), 2.5f * sqrt(2), 2.5f * sqrt(2)), pos, rad, p, y, r));
}

void Drone::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); {
		glMultMatrixf(matrix);

		glColor3f(0.7f, 0.7f, 0.7f);
		glutSolidSphere(boundingRad, 32, 32);

		glPushMatrix(); {
			glTranslatef(0.0f, 0.0f, boundingRad - boundingRad / 2.0f + 0.5f);
			glColor3f(0.5f, 0.5f, 0.5f);
			glutSolidCube(boundingRad - 1.0f);

			
			glTranslatef(0.0f, 0.0f, boundingRad - boundingRad / 2.0f - (boundingRad / 10.0f)*1.5f);
			glPushMatrix(); {
				glColor3f(0.8f, 0.8f, 0.8f);
				glTranslatef(0.0f, 0.0f, boundingRad / 30.0f);
				glScalef(1.2f, 1.2f, 1.2f);
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
			glTranslatef(0.0f, 0.0f, boundingRad );
			arm.Render();
			glPopMatrix();
			glScalef(0.3f, 1.0f, 1.0f);
			glColor3f(0.6f, 0.6f, 0.6f);
			glutSolidSphere(boundingRad - 2.0f, 32, 32);
		}glPopMatrix();
		glPushMatrix(); {
			glTranslatef(boundingRad - 0.5f, 0.0f, 0.0f);
			glPushMatrix();
			glTranslatef(0.0f, 0.0f, boundingRad);
			arm.Render();
			glPopMatrix();
			glScalef(0.3f, 1.0f, 1.0f);
			glColor3f(0.6f, 0.6f, 0.6f);
			glutSolidSphere(boundingRad - 2.0f, 32, 32);
		}glPopMatrix();
	}glPopMatrix();
}

void Drone::Update()
{
	float bulletmtx[16];
	float tmpmtx[16];
	glMatrixMode(GL_MODELVIEW);
	glGetFloatv(GL_MODELVIEW_MATRIX, tmpmtx);
	glLoadIdentity();

	glTranslatef(position.x, position.y, position.z);
	glRotatef(pitch, 1.0f, 0.0f, 0.0f);
	glRotatef(yaw, 0.0f, 1.0f, 0.0f);
	glRotatef(roll, 0.0f, 0.0f, 1.0f);
	glTranslatef(-boundingRad + 0.5f, 0.0f, boundingRad);
	glGetFloatv(GL_MODELVIEW_MATRIX, bulletmtx);
	arm.Shot(false, bulletmtx);

	glLoadIdentity();
	glLoadMatrixf(tmpmtx);

	glMatrixMode(GL_MODELVIEW);
	glGetFloatv(GL_MODELVIEW_MATRIX, tmpmtx);
	glLoadIdentity();

	glTranslatef(position.x, position.y, position.z);
	glRotatef(pitch, 1.0f, 0.0f, 0.0f);
	glRotatef(yaw, 0.0f, 1.0f, 0.0f);
	glRotatef(roll, 0.0f, 0.0f, 1.0f);
	glTranslatef(boundingRad - 0.5f, 0.0f, boundingRad);
	glGetFloatv(GL_MODELVIEW_MATRIX, bulletmtx);
	arm.Shot(false, bulletmtx);

	glLoadIdentity();
	glLoadMatrixf(tmpmtx);

	this->Object::GenMatrix();
	for (unsigned int i = 0; i < cubeList.size(); ++i) {
		cubeList[i]->Object::GenMatrix();
	}
}
