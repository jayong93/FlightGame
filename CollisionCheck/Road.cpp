#include "Road.h"
#include "std.h"

Road::Road(float x, float y, float z, float w, float h, float pitch, float yaw, float roll) : CubeObject(vec3(w / 2.0, 0.1, h / 2.0), vec3(x, y, z), 0, pitch, yaw, roll), width(w), height(h)
{
	boundingRad = vec3(w / 2.0, 0, h / 2.0).GetSize();
	if (texture < 0)
	{
		GLubyte* bitData;
		BITMAPINFO* bInfo;
		bitData = LoadDIBitmap("road.bmp", &bInfo);

		GLuint t;
		glGenTextures(1, &t);
		texture = t;
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, bInfo->bmiHeader.biWidth, bInfo->bmiHeader.biHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitData);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		tWidth = bInfo->bmiHeader.biWidth;
		tHeight = bInfo->bmiHeader.biHeight;
	}
}

Road::Road(const vec3 & p1, const vec3 & p2, float w) : CubeObject(vec3(w / 2.0, 0.1, (p2-p1).GetSize() / 2.0), p1 + ((p2 - p1) / 2.0), vec3(w / 2.0, 0.1, (p2 - p1).GetSize() / 2.0).GetSize(), 0, 0, 0), width(w), height((p2 - p1).GetSize())
{
	position.y = 0;

	vec3 dir = p2 - p1, unit(0, 0, -1);
	dir.y = 0;
	dir.Normalize();
	if (dir.x < 0)
		yaw = acosf(dir.Dot(unit)) * 180 / 3.14;
	else
		yaw = -acosf(dir.Dot(unit)) * 180 / 3.14;
	Object::GenMatrix();

	if (texture < 0)
	{
		GLubyte* bitData;
		BITMAPINFO* bInfo;
		bitData = LoadDIBitmap("road.bmp", &bInfo);

		GLuint t;
		glGenTextures(1, &t);
		texture = t;
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, bInfo->bmiHeader.biWidth, bInfo->bmiHeader.biHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitData);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		tWidth = bInfo->bmiHeader.biWidth;
		tHeight = bInfo->bmiHeader.biHeight;
	}
}

void Road::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	{
		glMultMatrixf(matrix);
		glColor3f(1, 1, 1);
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTranslatef(0, 0, height / 2.0);

		glNormal3f(0, 1, 0);
		glBegin(GL_QUADS);
		for (int i = 0; i < int(height / width); ++i)
		{
			glTexCoord2f(0, 1);
			glVertex3f(-width / 2.0, 0, -width*(i + 1));
			glTexCoord2f(0, 0);
			glVertex3f(-width / 2.0, 0, -width*i);
			glTexCoord2f(1, 0);
			glVertex3f(width / 2.0, 0, -width*i);
			glTexCoord2f(1, 1);
			glVertex3f(width / 2.0, 0, -width*(i + 1));
		}
		float t = height - width*int(height / width);
		if (t > 0)
		{
			glTexCoord2f(0, t / width);
			glVertex3f(-width / 2.0, 0, -height);
			glTexCoord2f(0, 0);
			glVertex3f(-width / 2.0, 0, -height + t);
			glTexCoord2f(1, 0);
			glVertex3f(width / 2.0, 0, -height + t);
			glTexCoord2f(1, t / width);
			glVertex3f(width / 2.0, 0, -height);
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();
}

int Road::texture = -1;
int Road::tWidth = 0;
int Road::tHeight = 0;