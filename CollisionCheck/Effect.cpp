#include "Effect.h"
#include "std.h"

void Flame::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(position.x, position.y, position.z);
	glColor3f(color.x, color.y, color.z);
	glutSolidSphere(fRaudius, 16, 16);

	glPopMatrix();
}

void Flame::Update(float frameTime)
{
	position.x += direction.x*fspeed*frameTime;
	position.y += direction.y*fspeed*frameTime;
	position.z += direction.z*fspeed*frameTime;
	
	fspeed += fspeedRate*frameTime;
	fRaudius -= 2.0*frameTime;
	if (fRaudius < 0.0f) alive = false;
}

void Spark::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	
	glTranslatef(position.x, position.y, position.z);

	glColor3f(color.x, color.y, color.z);
	//glutSolidCube(0.3f);
	//glBegin(GL_QUADS);
	glBegin(GL_LINE_LOOP);
	glVertex3f(Point[0][0], Point[0][1],0.0f);
	glVertex3f(Point[3][0], Point[3][1],0.0f);
	glVertex3f(Point[1][0], Point[1][1],0.0f);
	glVertex3f(Point[4][0], Point[4][1],0.0f);
	glVertex3f(Point[2][0], Point[2][1],0.0f);
	glEnd();
	glPopMatrix();
}

void Spark::Update(float frameTime)
{
	position.x += direction.x*fspeed*frameTime;
	position.y += direction.y*fspeed*frameTime;
	position.z += direction.z*fspeed*frameTime;

	timer += frameTime;
	if (timer > time) alive = false;
}
