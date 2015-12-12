#include "Ring.h"
#include "std.h"

void Ring::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	{
<<<<<<< HEAD
		item.Render();
=======
>>>>>>> refs/remotes/origin/SoYun
		glMultMatrixf(matrix);
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
		float tmp[16];
		glMatrixMode(GL_MODELVIEW);
		glGetFloatv(GL_MODELVIEW_MATRIX, tmp);
		glLoadMatrixf(matrix);
		glRotatef(60 * frameTime, 0, 0, 1);
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
		glLoadMatrixf(tmp);

		for (auto& c : cubeList)
		{
			c->UpdateMatrix(matrix);
		}
	}
<<<<<<< HEAD
	item.Update(frameTime);
}

bool Ring::CollisionCheck(const Object * obj) const
=======
}

bool Ring::CollisionCheck(const CubeObject * obj) const
>>>>>>> refs/remotes/origin/SoYun
{
	if (Object::CollisionCheck(obj))
	{
		for (auto& c : cubeList)
		{
			if (obj->CollisionCheck(c))
<<<<<<< HEAD
=======
				std::cout << "CollisionWithRing" << std::endl;
>>>>>>> refs/remotes/origin/SoYun
				return true;
		}
	}
	return false;
}

<<<<<<< HEAD
Ring::Ring(float x, float y, float z, float w, float h, float d, float angle, bool rotate) : Object(vec3(x, y, z), vec3(w, h, d).GetSize(), 0, angle, 0), isRotate(rotate), item(vec3(w / 6.0, h / 6.0, h / 6.0), vec3(x, y, z), vec3(w / 4.0, h / 4.0, d / 4.0).GetSize(), 0, 0, 0)
=======
Ring::Ring(float x, float y, float z, float w, float h, float d, float angle, bool rotate) : Object(vec3(x, y, z), vec3(w, h, d).GetSize(), 0, angle, 0), isRotate(rotate)
>>>>>>> refs/remotes/origin/SoYun
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
	glTranslatef(relativePos.x, relativePos.y, relativePos.z);
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

<<<<<<< HEAD
float Item::pointColor[8][3] = 
{
	{0,0,0}, {1,0,0}, {0,1,0}, {0,0,1},
	{1,0,1}, {1,1,0}, {0,1,1}, {1,1,1}
};

=======
>>>>>>> refs/remotes/origin/SoYun
void Item::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	{
<<<<<<< HEAD
		glMultMatrixf(matrix);
		glScalef(extent.x*2, extent.y*2, extent.z*2);
		glBegin(GL_QUADS);
		{
			glNormal3f(0, 0, -1);
			glColor3fv(pointColor[0]);
			glVertex3f(-0.5, -0.5, -0.5);
			glColor3fv(pointColor[2]);
			glVertex3f(-0.5, 0.5, -0.5);
			glColor3fv(pointColor[5]);
			glVertex3f(0.5, 0.5, -0.5);
			glColor3fv(pointColor[1]);
			glVertex3f(0.5, -0.5, -0.5);

			glNormal3f(-1, 0, 0);
			glColor3fv(pointColor[0]);
			glVertex3f(-0.5, -0.5, -0.5);
			glColor3fv(pointColor[3]);
			glVertex3f(-0.5, -0.5, 0.5);
			glColor3fv(pointColor[6]);
			glVertex3f(-0.5, 0.5, 0.5);
			glColor3fv(pointColor[2]);
			glVertex3f(-0.5, 0.5, -0.5);

			glNormal3f(0, -1, 0);
			glColor3fv(pointColor[0]);
			glVertex3f(-0.5, -0.5, -0.5);
			glColor3fv(pointColor[1]);
			glVertex3f(0.5, -0.5, -0.5);
			glColor3fv(pointColor[4]);
			glVertex3f(0.5, -0.5, 0.5);
			glColor3fv(pointColor[3]);
			glVertex3f(-0.5, -0.5, 0.5);

			glNormal3f(0, 0, 1);
			glColor3fv(pointColor[6]);
			glVertex3f(-0.5, 0.5, 0.5);
			glColor3fv(pointColor[3]);
			glVertex3f(-0.5, -0.5, 0.5);
			glColor3fv(pointColor[4]);
			glVertex3f(0.5, -0.5, 0.5);
			glColor3fv(pointColor[7]);
			glVertex3f(0.5, 0.5, 0.5);

			glNormal3f(0, 1, 0);
			glColor3fv(pointColor[6]);
			glVertex3f(-0.5, 0.5, 0.5);
			glColor3fv(pointColor[7]);
			glVertex3f(0.5, 0.5, 0.5);
			glColor3fv(pointColor[5]);
			glVertex3f(0.5, 0.5, -0.5);
			glColor3fv(pointColor[2]);
			glVertex3f(-0.5, 0.5, -0.5);

			glNormal3f(1, 0, 0);
			glColor3fv(pointColor[7]);
			glVertex3f(0.5, 0.5, 0.5);
			glColor3fv(pointColor[4]);
			glVertex3f(0.5, -0.5, 0.5);
			glColor3fv(pointColor[1]);
			glVertex3f(0.5, -0.5, -0.5);
			glColor3fv(pointColor[5]);
			glVertex3f(0.5, 0.5, -0.5);
		}
		glEnd();
=======

>>>>>>> refs/remotes/origin/SoYun
	}
	glPopMatrix();
}

<<<<<<< HEAD
void Item::Update(float frameTime)
{
	this->Rotate(0, 60 * frameTime, 0);
}

=======
>>>>>>> refs/remotes/origin/SoYun
RotateRing::RotateRing(float x, float y, float z, float w, float h, float d, float r) : Ring(x, y, z, w, h, d), rad(r), angle(0)
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
