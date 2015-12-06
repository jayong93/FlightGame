#include "Unit.h"
#include "std.h"

Unit::Unit(vec3 & pos, float rad, float p, float y, float r) : Object(pos, rad, p, y, r), velocity(0, 0, 0), isAlly(false), isAlive(true)
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
	cubeList.push_back(new CubeObject(vec3(5*sqrt(2), 5 * sqrt(2), 5 * sqrt(2)), pos, rad, p, y, r));
}

void Drone::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(matrix);
	glColor3f(0.7f, 0.7f, 0.7f);
	glutSolidSphere(boundingRad, 32, 32);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, boundingRad - boundingRad/2.0f + 1.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	glutSolidCube(boundingRad - 2.0f);
	glColor3f(0.3f, 0.3f, 1.0f);
	glTranslatef(0.0f, 0.0f, boundingRad - boundingRad / 2.0f - (boundingRad/10.0f)*1.2f );
	glPushMatrix();
	glTranslatef(boundingRad - boundingRad / 2.0f - (boundingRad / 10.0f)*2.2f, boundingRad - boundingRad / 2.0f - (boundingRad / 10.0f)*2.5f, 0.0f);
	glutSolidSphere(boundingRad / 10.0f, 32, 32);
	glPopMatrix();
	glPushMatrix();
	glColor3f(0.3f, 1.0f, 0.3f);
	glTranslatef(-(boundingRad - boundingRad / 2.0f - (boundingRad / 10.0f)*2.2f), -(boundingRad - boundingRad / 2.0f - (boundingRad / 10.0f)*2.5f), 0.0f);
	glutSolidSphere(boundingRad / 10.0f, 32, 32);
	glPopMatrix();
	glPopMatrix();
	glColor3f(0.6f, 0.6f, 0.6f);
	glPushMatrix();
	glTranslatef(-boundingRad + 1, 0.0f, 0.0f);
	glScalef(0.3f, 1.0f, 1.0f);
	glutSolidSphere(boundingRad - 3.0f, 32, 32);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(boundingRad - 1, 0.0f, 0.0f);
	glScalef(0.3f, 1.0f, 1.0f);
	glutSolidSphere(boundingRad - 3.0f, 32, 32);
	glPopMatrix();
	glPopMatrix();
}

void Drone::Update()
{
}
