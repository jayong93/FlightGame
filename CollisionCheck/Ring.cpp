#include "Ring.h"
#include "std.h"

void Ring::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	{
		glMultMatrixf(matrix);
		for (auto& c : cubeList)
		{
			c->Render();
		}
		glRotatef(pitch, -1, 0, 0);
		glRotatef(yaw, 0, -1, 0);
		glRotatef(roll, 0, 0, -1);
		item.Render();
	}
	glPopMatrix();
}

void Ring::Update(float frameTime)
{
	if (isRotate)
	{
		this->Rotate(0, 0, 60 * frameTime);
		for (auto& c : cubeList)
		{
			c->Rotate(0, 0, 60 * frameTime);
		}
	}
	item.Update(frameTime);
}

Ring::Ring(float x, float y, float z, float w, float h, float d) : Object(vec3(x, y, z), vec3(w / 2.0, h / 2.0, d / 2.0).GetSize(), 0, 0, 0), isRotate(true), item(vec3(w / 4.0, h / 4.0, w / 4.0), vec3(x, y, z), vec3(w / 4.0, h / 4.0, d / 4.0).GetSize(), 0, 0, 0)
{
	// 테두리 생성

	float wx, wy, wz;

	wx = -w / 2.0 + d / 2.0;
	wy = 0;
	wz = 0;
	vec3 ext = vec3(d / 2.0, h / 2.0, d / 2.0);
	vec3 rPos = vec3(wx, wy, wz);
	cubeList.push_back(new RingEdge(ext, position + rPos, rPos, ext.GetSize(), 0, 0, 0));

	wx = 0;
	wy = h / 2.0 - d / 2.0;
	wz = 0;
	ext = vec3(w / 2.0, d / 2.0, d / 2.0);
	rPos = vec3(wx, wy, wz);
	cubeList.push_back(new RingEdge(ext, position + rPos, rPos, ext.GetSize(), 0, 0, 0));

	wx = w / 2.0 - d / 2.0;
	wy = 0;
	wz = 0;
	ext = vec3(d / 2.0, h / 2.0, d / 2.0);
	rPos = vec3(wx, wy, wz);
	cubeList.push_back(new RingEdge(ext, position + rPos, rPos, ext.GetSize(), 0, 0, 0));

	wx = 0;
	wy = -h / 2.0 + d / 2.0;
	wz = 0;
	ext = vec3(w / 2.0, d / 2.0, d / 2.0);
	rPos = vec3(wx, wy, wz);
	cubeList.push_back(new RingEdge(ext, position + rPos, rPos, ext.GetSize(), 0, 0, 0));
}

RingEdge::RingEdge(vec3 & ext, vec3 & pos, vec3& rPos, float rad, float p, float y, float r) : CubeObject(ext, pos, rad, p, y, r), relativePos(rPos)
{
}

void RingEdge::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(relativePos.x, relativePos.y, relativePos.z);
	glColor3f(1.0f, 1.0f, 0.0f);
	glScalef(2 * extent.x, 2 * extent.y, 2 * extent.z);
	glutSolidCube(1.0f);
	glPopMatrix();
}

Item::Item(vec3 & extent, vec3 & pos, float rad, float p, float y, float r) : CubeObject(extent, pos, rad, p, y, r)
{
}

void Item::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	{
		float color[8][3] =
		{
			{0,0,0},
			{1,0,0},
			{0,1,0},
			{0,0,1},
			{1,1,0},
			{1,0,1},
			{0,1,1},
			{1,1,1}
		};
		glScalef(extent.x * 2.0, extent.y * 2.0, extent.z * 2.0);
		glRotatef(yaw, 0, 1, 0);
		glTranslatef(-0.5, -0.5, -0.5);
		glBegin(GL_QUADS);
		{
			glColor3fv(color[0]);
			glVertex3fv(color[0]);

			glColor3fv(color[3]);
			glVertex3fv(color[3]);

			glColor3fv(color[6]);
			glVertex3fv(color[6]);

			glColor3fv(color[2]);
			glVertex3fv(color[2]);

			glColor3fv(color[0]);
			glVertex3fv(color[0]);

			glColor3fv(color[2]);
			glVertex3fv(color[2]);

			glColor3fv(color[4]);
			glVertex3fv(color[4]);

			glColor3fv(color[1]);
			glVertex3fv(color[1]);

			glColor3fv(color[0]);
			glVertex3fv(color[0]);

			glColor3fv(color[1]);
			glVertex3fv(color[1]);

			glColor3fv(color[5]);
			glVertex3fv(color[5]);

			glColor3fv(color[3]);
			glVertex3fv(color[3]);

			glColor3fv(color[3]);
			glVertex3fv(color[3]);

			glColor3fv(color[5]);
			glVertex3fv(color[5]);

			glColor3fv(color[7]);
			glVertex3fv(color[7]);

			glColor3fv(color[6]);
			glVertex3fv(color[6]);

			glColor3fv(color[2]);
			glVertex3fv(color[2]);

			glColor3fv(color[6]);
			glVertex3fv(color[6]);

			glColor3fv(color[7]);
			glVertex3fv(color[7]);

			glColor3fv(color[4]);
			glVertex3fv(color[4]);

			glColor3fv(color[5]);
			glVertex3fv(color[5]);

			glColor3fv(color[1]);
			glVertex3fv(color[1]);

			glColor3fv(color[4]);
			glVertex3fv(color[4]);

			glColor3fv(color[7]);
			glVertex3fv(color[7]);
		}
		glEnd();
	}
	glPopMatrix();
}

void Item::Update(float frameTime)
{
	yaw += 60 * frameTime;
}
