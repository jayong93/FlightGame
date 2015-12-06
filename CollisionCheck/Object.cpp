#include "Object.h"
#include "std.h"


float DotProduct(const vec3& v1, const vec3& v2) { return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z); }

float GetDistance(const vec3& v1, const vec3& v2) { return sqrt((v2.x - v1.x)*(v2.x - v1.x) + (v2.y - v1.y)*(v2.y - v1.y) + (v2.z - v1.z)*(v2.z - v1.z)); }

Object::Object(vec3& pos,float boundingRadius ,float p = 0.0f, float y = 0.0f, float r = 0.0f) : position(pos), boundingRad(boundingRadius), pitch(p), yaw(y), roll(r) {
	Object::GenMatrix();
}

void Object::GenMatrix()
{
	float tmpmtx[16];
	glMatrixMode(GL_MODELVIEW);
	glGetFloatv(GL_MODELVIEW_MATRIX, tmpmtx);
	glLoadIdentity();

	glTranslatef(position.x, position.y, position.z);
	glRotatef(pitch, 1.0f, 0.0f, 0.0f);
	glRotatef(yaw, 0.0f, 1.0f, 0.0f);
	glRotatef(roll, 0.0f, 0.0f, 1.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);

	glLoadIdentity();
	glLoadMatrixf(tmpmtx);
}

bool Object::CollisionCheck(const Object & obj)
{
	float d = GetDistance(this->position, obj.position);
	if (d < this->boundingRad + obj.boundingRad) return true;
	return false;
}

void Object::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(matrix);
	glColor3f(0.2f, 0.2f, 0.2f);
	glutWireSphere(boundingRad, 10, 10);
	glPopMatrix();
}

const vec3 & Object::GetPos() const
{
	return position;
}

const float * Object::GetMatrix() const
{
	return matrix;
}

void Object::Move(const vec3 & shift)
{
	position.x += shift.x; position.y += shift.y; position.z += shift.z;
	Object::GenMatrix();
}

void Object::Rotate(float pdel, float ydel, float rdel)
{
	pitch += pdel; yaw += ydel; roll += rdel;
	Object::GenMatrix();
}


CubeObject::CubeObject(vec3 & ext, vec3 & pos, float boundingRadius, float p, float y, float r) : Object::Object(pos, boundingRadius, p, y, r), extent(ext)
{
}

//위에서 GenMatrix 호출 완료됐다는 전제하에 호출
bool CubeObject::CollisionCheck(const CubeObject& box2)
{
	if (Object::CollisionCheck(box2)) {		//	바운딩 구가 충돌하면
		CubeObject& box1 = *this;
		vec3 T = vec3(box2.position.x - box1.position.x, box2.position.y - box1.position.y, box2.position.z - box1.position.z);
		float R[3][3];					//	matrix R = A^T * B ,r_{ij} = Dot(A_i,B_j)
		float absR[3][3];				//	|r_{ij}|
		float AdT[3];					//	Dot(A_i,D)
		float Da, Db, D;				//	interval radius and distance between centers

		

		//A0
		R[0][0] = DotProduct(vec3(box1.matrix[0], box1.matrix[4], box1.matrix[8]), vec3(box2.matrix[0], box2.matrix[4], box2.matrix[8]));
		R[0][1] = DotProduct(vec3(box1.matrix[0], box1.matrix[4], box1.matrix[8]), vec3(box2.matrix[1], box2.matrix[5], box2.matrix[9]));
		R[0][2] = DotProduct(vec3(box1.matrix[0], box1.matrix[4], box1.matrix[8]), vec3(box2.matrix[2], box2.matrix[6], box2.matrix[10]));
		AdT[0] = DotProduct(vec3(box1.matrix[0], box1.matrix[4], box1.matrix[8]), T);
		absR[0][0] = (float)fabs(R[0][0]);
		absR[0][1] = (float)fabs(R[0][1]);
		absR[0][2] = (float)fabs(R[0][2]);
		D = (float)fabs(AdT[0]);
		Db = box2.extent.x * absR[0][0] + box2.extent.y * absR[0][1] + box2.extent.z * absR[0][2];
		Da = box1.extent.x;
		if (D > Da + Db) return false;

		//A1
		R[1][0] = DotProduct(vec3(box1.matrix[1], box1.matrix[5], box1.matrix[9]), vec3(box2.matrix[0], box2.matrix[4], box2.matrix[8]));
		R[1][1] = DotProduct(vec3(box1.matrix[1], box1.matrix[5], box1.matrix[9]), vec3(box2.matrix[1], box2.matrix[5], box2.matrix[9]));
		R[1][2] = DotProduct(vec3(box1.matrix[1], box1.matrix[5], box1.matrix[9]), vec3(box2.matrix[2], box2.matrix[6], box2.matrix[10]));
		AdT[1] = DotProduct(vec3(box1.matrix[1], box1.matrix[5], box1.matrix[9]), T);
		absR[1][0] = (float)fabs(R[1][0]);
		absR[1][1] = (float)fabs(R[1][1]);
		absR[1][2] = (float)fabs(R[1][2]);
		D = (float)fabs(AdT[1]);
		Db = box2.extent.x * absR[1][0] + box2.extent.y * absR[1][1] + box2.extent.z * absR[1][2];
		Da = box1.extent.y;
		if (D > Da + Db) return false;

		//A2
		R[2][0] = DotProduct(vec3(box1.matrix[2], box1.matrix[6], box1.matrix[10]), vec3(box2.matrix[0], box2.matrix[4], box2.matrix[8]));
		R[2][1] = DotProduct(vec3(box1.matrix[2], box1.matrix[6], box1.matrix[10]), vec3(box2.matrix[1], box2.matrix[5], box2.matrix[9]));
		R[2][2] = DotProduct(vec3(box1.matrix[2], box1.matrix[6], box1.matrix[10]), vec3(box2.matrix[2], box2.matrix[6], box2.matrix[10]));
		AdT[2] = DotProduct(vec3(box1.matrix[2], box1.matrix[6], box1.matrix[10]), T);
		absR[2][0] = (float)fabs(R[2][0]);
		absR[2][1] = (float)fabs(R[2][1]);
		absR[2][2] = (float)fabs(R[2][2]);
		D = (float)fabs(AdT[2]);
		Db = box2.extent.x * absR[2][0] + box2.extent.y * absR[2][1] + box2.extent.z * absR[2][2];
		Da = box1.extent.z;
		if ( D > Da + Db ) return false;

		//B0
		D = (float)fabs(DotProduct(vec3(box2.matrix[0], box2.matrix[4], box2.matrix[8]), T));
		Da = box1.extent.x * absR[0][0] + box1.extent.y * absR[1][0] + box1.extent.z * absR[2][0];
		Db = box2.extent.x;
		if ( D > Da + Db)return false;

		//B1
		D = (float)fabs(DotProduct(vec3(box2.matrix[1], box2.matrix[5], box2.matrix[9]), T));
		Da = box1.extent.x * absR[0][1] + box1.extent.y * absR[1][1] + box1.extent.z * absR[2][1];
		Db = box2.extent.y;
		if ( D > Da + Db)return false;

		//B2
		D = (float)fabs(DotProduct(vec3(box2.matrix[2], box2.matrix[6], box2.matrix[10]), T));
		Da = box1.extent.x * absR[0][2] + box1.extent.y * absR[1][2] + box1.extent.z * absR[2][2];
		Db = box2.extent.z;
		if ( D > Da + Db)return false;

		//A0xB0
		D = (float)fabs(AdT[2] * R[1][0] - AdT[1] * R[2][0]);
		Da = box1.extent.y * absR[2][0] + box1.extent.z * absR[1][0];
		Db = box2.extent.y * absR[0][2] + box2.extent.z * absR[0][1];
		if ( D > Da + Db)return false;

		//A0xB1
		D = (float)fabs(AdT[2] * R[1][1] - AdT[1] * R[2][1]);
		Da = box1.extent.y * absR[2][1] + box1.extent.z * absR[1][1];
		Db = box2.extent.x * absR[0][2] + box2.extent.z * absR[0][0];
		if (D > Da + Db)return false;

		//A0xB2
		D = (float)fabs(AdT[2] * R[1][2] - AdT[1] * R[2][2]);
		Da = box1.extent.y * absR[2][2] + box1.extent.z * absR[1][2];
		Db = box2.extent.x * absR[0][1] + box2.extent.y * absR[0][0];
		if (D > Da + Db)return false;

		//A1xB0
		D = (float)fabs(AdT[0] * R[2][0] - AdT[2] * R[0][0]);
		Da = box1.extent.x * absR[2][0] + box1.extent.z * absR[0][0];
		Db = box2.extent.y * absR[1][2] + box2.extent.z * absR[1][1];
		if (D > Da + Db)return false;

		//A1xB1
		D = (float)fabs(AdT[0] * R[2][1] - AdT[2] * R[0][1]);
		Da = box1.extent.x * absR[2][1] + box1.extent.z * absR[0][1];
		Db = box2.extent.x * absR[1][2] + box2.extent.z * absR[1][0];
		if (D > Da + Db)return false;

		//A1xB2
		D = (float)fabs(AdT[0] * R[2][2] - AdT[2] * R[0][2]);
		Da = box1.extent.x * absR[2][2] + box1.extent.z * absR[0][2];
		Db = box2.extent.x * absR[1][1] + box2.extent.y * absR[1][0];
		if (D > Da + Db)return false;

		//A2xB0
		D = (float)fabs(AdT[1] * R[0][0] - AdT[0] * R[1][0]);
		Da = box1.extent.x * absR[1][0] + box1.extent.y * absR[0][0];
		Db = box2.extent.y * absR[2][2] + box2.extent.z * absR[2][1];
		if (D > Da + Db)return false;

		//A2xB1
		D = (float)fabs(AdT[1] * R[0][1] - AdT[0] * R[1][1]);
		Da = box1.extent.x * absR[1][1] + box1.extent.y * absR[0][1];
		Db = box2.extent.x * absR[2][2] + box2.extent.z * absR[2][0];
		if (D > Da + Db)return false;

		//A2xB2
		D = (float)fabs(AdT[1] * R[0][2] - AdT[0] * R[1][2]);
		Da = box1.extent.x * absR[1][2] + box1.extent.y * absR[0][2];
		Db = box2.extent.x * absR[2][1] + box2.extent.y * absR[2][0];
		if (D > Da + Db)return false;

		return true;
	}
	return false;
}

void CubeObject::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(matrix);
	glColor3f(0.2f, 0.2f, 0.2f);
	glutWireSphere(boundingRad, 10, 10);
	glScalef(2 *extent.x, 2 * extent.y, 2 * extent.z);
	glColor3f(1.0f, 0.0f, 0.0f);
	glutWireCube(1.0f);
	glPopMatrix();
}

Building::Building(vec3 & ext, vec3 & pos, float boundingRadius, float p, float y, float r) : CubeObject::CubeObject(ext, pos, boundingRadius, p, y, r)
{
}

void Building::Render()
{
	CubeObject::Render();
}

void Building::Update()
{
	Object::GenMatrix();
}
