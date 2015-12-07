#pragma once
#include "Vector.h"

//Object �ٿ�� �� ������ �ִ�.
//CubeObject OOBB ������ ������ �ִ�.
//���⿡ �߰��� �ǹ�(������ ��ü)�� ��� �浹üũ�� ����Ʈ�� (depth 2~3 )�� �̿��Ѵ�.

enum ObjectType { OBJ_PLAYER, OBJ_DRONE, OBJ_BUILDING };

float DotProduct(const vec3& v1, const vec3& v2);

float GetDistance(const vec3& v1, const vec3& v2);


//Object�� CubeObject�� Render�Լ��� �浹üũ�� ���� ��. ���߿��� ���� ����
class Object {
protected:
	vec3 position;
	float boundingRad;
	float pitch, yaw, roll;

	float matrix[16];
public:
	Object(vec3&, float, float, float, float);

	void GenMatrix();

	virtual bool CollisionCheck(const Object&);

	virtual void Render();

	virtual void Move(const vec3&);

	virtual void Rotate(float, float, float);

	virtual void Update(float frameTime) { }

	virtual void SetDes() { }

	const vec3& GetPos() const;

	void GetMatrix(float* m) const;

	void SetMatrix(const float* m);
};

class CubeObject : public Object {
protected:
	vec3 extent;
	float alpha;
public:
	CubeObject(vec3&, vec3&, float, float, float, float);

	virtual bool CollisionCheck(const CubeObject&);

	virtual void Render();

	virtual bool CollisionCheckRay(const vec3& starat, const vec3& end, float& intersection_distance);

	void SetAlpha(float a) { alpha = a; }

	float GetAlpha() const { return alpha; }
};

//������ ��ü�� �����Ͽ� Update�Լ� ����
class Building : public CubeObject {
	vec3 color;
public:
	Building(vec3&, vec3&, float, float, float, float);

	void SetColor(float, float, float);
	void Render();
};