#pragma once
#include "Unit.h"

class Player : public Unit
{
public:
	Player(float x, float y, float z);

	virtual bool ColiisionCheck(const Object& obj);
	virtual void Render();
	void Update();

private:
	CubeObject* parts[5];
};