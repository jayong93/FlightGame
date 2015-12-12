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
