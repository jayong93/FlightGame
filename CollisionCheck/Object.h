#pragma once
#include "Vector.h"

//Object 바운딩 구 가지고 있다.
//CubeObject OOBB 로직도 가지고 있다.
//여기에 추가로 건물(정적인 객체)의 경우 충돌체크에 쿼드트리 (depth 2~3 )를 이용한다.

enum ObjectType { OBJ_PLAYER, OBJ_DRONE, OBJ_BUILDING };

float DotProduct(const vec3& v1, const vec3& v2);

float GetDistance(const vec3& v1, const vec3& v2);


//Object와 CubeObject의 Render함수는 충돌체크를 위한 것. 나중에는 없앨 예정
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

//동적인 객체란 가정하에 Update함수 생성
class Building : public CubeObject {
	vec3 color;
public:
	Building(vec3&, vec3&, float, float, float, float);

	void SetColor(float, float, float);
	void Render();
};