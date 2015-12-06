#include "Player.h"

Player::Player(float x, float y, float z) : Unit(vec3(x, y, z), 30, 0, 0, 0)
{
	
}

bool Player::ColiisionCheck(const Object & obj)
{
	return false;
}

void Player::Render()
{
}

void Player::Update()
{
}
