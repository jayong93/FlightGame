#include "Player.h"
#include "std.h"

Player::Player(float x, float y, float z) : Unit(vec3(x, y, z), 30, 0, 0, 0)
{
	cubeList.push_back(new PlayerBody(position, vec3(0,0,-2)));
	vec3 pos = position;

	cubeList.push_back(new PlayerWing(pos, vec3(0,2.5,5)));

	cubeList.push_back(new PlayerWing(pos, vec3(-4,0,5)));

	cubeList.push_back(new PlayerWing(pos, vec3(4,0,5)));
}

bool Player::ColiisionCheck(const Object & obj)
{
	//if (Object::CollisionCheck(obj))
	//{
	//	for (auto& c : cubeList)
	//	{
	//		c->CollisionCheck(obj);
	//	}
	//}
	return false;
}

void Player::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	{
		glMultMatrixf(matrix);
		for (auto c : cubeList)
		{
			c->Render();
		}
	}
	glPopMatrix();
}

void Player::Move(const vec3 & dir)
{
	position = position + dir;
	for (auto& c : cubeList)
	{
		// 충돌체크를 위해 꼭 해줘야 함
		c->Move(dir);
	}
}

void Player::Rotate(float pitch, float yaw, float roll)
{
	float mat[16], tmp[16];
	glMatrixMode(GL_MODELVIEW);
	glGetFloatv(GL_MODELVIEW_MATRIX, tmp);
	this->GetMatrix(mat);
	glLoadMatrixf(mat);
	glRotatef(pitch, 1, 0, 0);
	glRotatef(yaw, 0, 1, 0);
	glRotatef(roll, 0, 0, 1);
	glGetFloatv(GL_MODELVIEW_MATRIX, mat);
	this->SetMatrix(mat);
	glLoadMatrixf(tmp);

	float speed = velocity.GetSize();
	vec3 unit(0, 0, -1);
	unit = unit.MultMatrix(mat);
	velocity = unit*speed;
}

void Player::Update()
{
}

PlayerBody::PlayerBody(const vec3& oPos, const vec3& rPos) : CubeObject(vec3(2, 1.25, 7), oPos + rPos, 7, 0, 0, 0), relativePos(rPos)
{
}

void PlayerBody::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	{
		glTranslatef(relativePos.x, relativePos.y, relativePos.z);
		glScalef(extent.x * 2, extent.y * 2, extent.z * 2);
		glColor3f(1, 0, 0);
		glutSolidCube(1);
	}
	glPopMatrix();
}

PlayerWing::PlayerWing(const vec3 & oPos, const vec3& rPos) : CubeObject(vec3(2, 1.25, 4), oPos + rPos, 4, 0, 0, 0), relativePos(rPos)
{
}

void PlayerWing::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	{
		glTranslatef(relativePos.x, relativePos.y, relativePos.z);
		glScalef(extent.x * 2, extent.y * 2, extent.z * 2);
		glColor3f(1, 0.5, 0.5);
		glutSolidCube(1);
	}
	glPopMatrix();
}
