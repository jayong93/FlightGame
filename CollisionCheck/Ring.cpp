#include "Ring.h"
#include "std.h"

void Ring::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	{
		for (auto& c : cubeList)
		{
			c->Render();
		}
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
			c->UpdateMatrix(matrix);
		}
	}
}

Ring::Ring(float x, float y, float z, float w, float h, float d, float angle, bool rotate) : Object(vec3(x, y, z), vec3(w, h, d).GetSize(), 0, angle, 0), isRotate(rotate)
{
	// 테두리 생성

	float wx, wy, wz;

	wx = - w / 2.0 + d / 2.0;
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
	wy = - h / 2.0 + d / 2.0;
	wz = 0;
	ext = vec3(w / 2.0, d / 2.0, d / 2.0);
	rPos = vec3(wx, wy, wz);
	cubeList.push_back(new RingEdge(ext, position + rPos, rPos, ext.GetSize(), 0, 0, 0));

	for (auto& c : cubeList)
	{
		c->UpdateMatrix(matrix);
	}
}

RingEdge::RingEdge(vec3 & ext, vec3 & pos, vec3& rPos, float rad, float p, float y, float r) : CubeObject(ext, pos, rad, p, y, r), relativePos(rPos)
{
}

void RingEdge::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(matrix);
	glColor3f(1.0f, 1.0f, 0.0f);
	glScalef(2 * extent.x, 2 * extent.y, 2 * extent.z);
	glutSolidCube(1.0f);
	glPopMatrix();
}

void RingEdge::UpdateMatrix(float * pMatrix)
{
	float tmp[16];
	glMatrixMode(GL_MODELVIEW);
	glGetFloatv(GL_MODELVIEW_MATRIX, tmp);
	glLoadMatrixf(pMatrix);
	glTranslatef(relativePos.x, relativePos.y, relativePos.z);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	glLoadMatrixf(tmp);
}

Item::Item(vec3 & extent, vec3 & pos, float rad, float p, float y, float r) : CubeObject(extent, pos, rad, p, y, r)
{
}

void Item::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	{

	}
	glPopMatrix();
}

RotateRing::RotateRing(float x, float y, float z, float w, float h, float d, float r) : Ring(x,y,z,w,h,d), rad(r), angle(0)
{
	float tmp[16];

	glMatrixMode(GL_MODELVIEW);
	glGetFloatv(GL_MODELVIEW_MATRIX, tmp);
	glLoadMatrixf(matrix);
	glTranslatef(0, rad, 0);
	glGetFloatv(GL_MODELVIEW_MATRIX, ringMat);
	glLoadMatrixf(tmp);

	for (auto& c : cubeList)
	{
		c->UpdateMatrix(matrix);
	}
}

void RotateRing::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	{
		glMultMatrixf(ringMat);
		for (auto& c : cubeList)
		{
			c->Render();
		}
	}
	glPopMatrix();
}

void RotateRing::Update(float frameTime)
{
	angle += 60 * frameTime;
	if (angle >= 360) angle -= 360;

	float tmp[16];

	glMatrixMode(GL_MODELVIEW);
	glGetFloatv(GL_MODELVIEW_MATRIX, tmp);
	glLoadMatrixf(matrix);
	glRotatef(angle, 0, 0, 1);
	glTranslatef(0, rad, 0);
	glGetFloatv(GL_MODELVIEW_MATRIX, ringMat);
	glLoadMatrixf(tmp);

	for (auto& c : cubeList)
	{
		c->UpdateMatrix(matrix);
	}
}
