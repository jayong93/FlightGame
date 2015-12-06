#include "Road.h"
#include "std.h"

Road::Road(float x, float y, float z, float w, float h) : CubeObject(vec3(w / 2.0, 0.1, h / 2.0), vec3(x, y, z), 0, 0, 0, 0)
{
	boundingRad = w > h ? w / 2.0 : h / 2.0;
	if (texture < 0)
	{
		GLubyte* bitData;
		BITMAPINFO* bInfo;
		bitData = LoadDIBitmap("road_texture.bmp", &bInfo);

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, bInfo->bmiHeader.biWidth, bInfo->bmiHeader.biHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitData);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
}

void Road::Render()
{
}

unsigned int Road::texture = -1;