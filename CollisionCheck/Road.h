#pragma once
#include "Object.h"

class Road : public CubeObject
{
public:
	Road(float x, float y, float z, float w, float h);

	virtual void Render();

private:
	static unsigned int texture;
};