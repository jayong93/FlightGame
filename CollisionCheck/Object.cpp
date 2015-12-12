#include "Object.h"
#include "std.h"


float DotProduct(const vec3& v1, const vec3& v2) { return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z); }

float GetDistance(const vec3& v1, const vec3& v2) { return sqrt((v2.x - v1.x)*(v2.x - v1.x) + (v2.y - v1.y)*(v2.y - v1.y) + (v2.z - v1.z)*(v2.z - v1.z)); }

Object::Object(vec3& pos, float boundingRadius, float p = 0.0f, float y = 0.0f, float r = 0.0f) : position(pos), boundingRad(boundingRadius), pitch(p), yaw(y), roll(r) {
	Object::GenMatrix();
}

void Object::GenMatrix()
{
	float tmpmtx[16];
	glMatrixMode(GL_MODELVIEW);
	glGetFloatv(GL_MODELVIEW_MATRIX, tmpmtx);
	glLoadIdentity();

	glTranslatef(position.x, position.y, position.z);
	glRotatef(roll, 0.0f, 0.0f, 1.0f);
	glRotatef(yaw, 0.0f, 1.0f, 0.0f);
	glRotatef(pitch, 1.0f, 0.0f, 0.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);

	glLoadIdentity();
	glLoadMatrixf(tmpmtx);
}

bool Object::CollisionCheck(const Object* obj) const
{
	vec3 p(this->matrix[12], this->matrix[13], this->matrix[14]);
	vec3 o(obj->matrix[12], obj->matrix[13], obj->matrix[14]);
	float d = GetDistance(p, o);
	if (d < this->boundingRad + obj->boundingRad) return true;
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

const vec3 & Object::GetPos() const
{
	return position;
}

void Object::GetMatrix(float* m) const
{
	memcpy_s(m, sizeof(float) * 16, matrix, sizeof(float) * 16);
}

void Object::SetMatrix(const float * m)
{
	memcpy_s(matrix, sizeof(float) * 16, m, sizeof(float) * 16);
}


CubeObject::CubeObject(vec3 & ext, vec3 & pos, float boundingRadius, float p, float y, float r) : Object::Object(pos, boundingRadius, p, y, r), extent(ext), alpha(1)
{
}

//위에서 GenMatrix 호출 완료됐다는 전제하에 호출
bool CubeObject::CollisionCheck(const CubeObject* b) const
{
	if (Object::CollisionCheck(b)) {		//	바운딩 구가 충돌하면
		const CubeObject& box2 = *b;
		const CubeObject& box1 = *this;
		vec3 T = vec3(box2.matrix[12] - box1.matrix[12], box2.matrix[13] - box1.matrix[13], box2.matrix[14] - box1.matrix[14]);
		float R[3][3];					//	matrix R = A^T * B ,r_{ij} = Dot(A_i,B_j)
		float absR[3][3];				//	|r_{ij}|
		float AdT[3];					//	Dot(A_i,D)
		float Da, Db, D;				//	interval radius and distance between centers



										//A0
		R[0][0] = DotProduct(vec3(box1.matrix[0], box1.matrix[1], box1.matrix[2]), vec3(box2.matrix[0], box2.matrix[1], box2.matrix[2]));
		R[0][1] = DotProduct(vec3(box1.matrix[0], box1.matrix[1], box1.matrix[2]), vec3(box2.matrix[4], box2.matrix[5], box2.matrix[6]));
		R[0][2] = DotProduct(vec3(box1.matrix[0], box1.matrix[1], box1.matrix[2]), vec3(box2.matrix[8], box2.matrix[9], box2.matrix[10]));
		AdT[0] = DotProduct(vec3(box1.matrix[0], box1.matrix[1], box1.matrix[2]), T);
		absR[0][0] = (float)fabs(R[0][0]);
		absR[0][1] = (float)fabs(R[0][1]);
		absR[0][2] = (float)fabs(R[0][2]);
		D = (float)fabs(AdT[0]);
		Db = box2.extent.x * absR[0][0] + box2.extent.y * absR[0][1] + box2.extent.z * absR[0][2];
		Da = box1.extent.x;
		if (D > Da + Db) return false;

		//A1
		R[1][0] = DotProduct(vec3(box1.matrix[4], box1.matrix[5], box1.matrix[6]), vec3(box2.matrix[0], box2.matrix[1], box2.matrix[2]));
		R[1][1] = DotProduct(vec3(box1.matrix[4], box1.matrix[5], box1.matrix[6]), vec3(box2.matrix[4], box2.matrix[5], box2.matrix[6]));
		R[1][2] = DotProduct(vec3(box1.matrix[4], box1.matrix[5], box1.matrix[6]), vec3(box2.matrix[8], box2.matrix[9], box2.matrix[10]));
		AdT[1] = DotProduct(vec3(box1.matrix[4], box1.matrix[5], box1.matrix[6]), T);
		absR[1][0] = (float)fabs(R[1][0]);
		absR[1][1] = (float)fabs(R[1][1]);
		absR[1][2] = (float)fabs(R[1][2]);
		D = (float)fabs(AdT[1]);
		Db = box2.extent.x * absR[1][0] + box2.extent.y * absR[1][1] + box2.extent.z * absR[1][2];
		Da = box1.extent.y;
		if (D > Da + Db) return false;

		//A2
		R[2][0] = DotProduct(vec3(box1.matrix[8], box1.matrix[9], box1.matrix[10]), vec3(box2.matrix[0], box2.matrix[1], box2.matrix[2]));
		R[2][1] = DotProduct(vec3(box1.matrix[8], box1.matrix[9], box1.matrix[10]), vec3(box2.matrix[4], box2.matrix[5], box2.matrix[6]));
		R[2][2] = DotProduct(vec3(box1.matrix[8], box1.matrix[9], box1.matrix[10]), vec3(box2.matrix[8], box2.matrix[9], box2.matrix[10]));
		AdT[2] = DotProduct(vec3(box1.matrix[8], box1.matrix[9], box1.matrix[10]), T);
		absR[2][0] = (float)fabs(R[2][0]);
		absR[2][1] = (float)fabs(R[2][1]);
		absR[2][2] = (float)fabs(R[2][2]);
		D = (float)fabs(AdT[2]);
		Db = box2.extent.x * absR[2][0] + box2.extent.y * absR[2][1] + box2.extent.z * absR[2][2];
		Da = box1.extent.z;
		if (D > Da + Db) return false;

		//B0
		D = (float)fabs(DotProduct(vec3(box2.matrix[0], box2.matrix[1], box2.matrix[2]), T));
		Da = box1.extent.x * absR[0][0] + box1.extent.y * absR[1][0] + box1.extent.z * absR[2][0];
		Db = box2.extent.x;
		if (D > Da + Db)return false;

		//B1
		D = (float)fabs(DotProduct(vec3(box2.matrix[4], box2.matrix[5], box2.matrix[6]), T));
		Da = box1.extent.x * absR[0][1] + box1.extent.y * absR[1][1] + box1.extent.z * absR[2][1];
		Db = box2.extent.y;
		if (D > Da + Db)return false;

		//B2
		D = (float)fabs(DotProduct(vec3(box2.matrix[8], box2.matrix[9], box2.matrix[10]), T));
		Da = box1.extent.x * absR[0][2] + box1.extent.y * absR[1][2] + box1.extent.z * absR[2][2];
		Db = box2.extent.z;
		if (D > Da + Db)return false;

		//A0xB0
		D = (float)fabs(AdT[2] * R[1][0] - AdT[1] * R[2][0]);
		Da = box1.extent.y * absR[2][0] + box1.extent.z * absR[1][0];
		Db = box2.extent.y * absR[0][2] + box2.extent.z * absR[0][1];
		if (D > Da + Db)return false;

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

bool CubeObject::CollisionCheckRay(const vec3 & start, const vec3 & end, float& intersection_distance)
{
	if (GetDistance(position, start) <= GetDistance(start, end)) {

		vec3 ray_direction = end - start;
		vec3 aabb_min = -extent;
		vec3 aabb_max = extent;

		float Min = 0.0f;		//	현재 까지 구해진 것 중 Greatest min
		float Max = 100000.0f;		//	Smallest max 

		vec3 OOBpos = position;
		vec3 delta = OOBpos - start;

		//OOB's X axis
		{
			vec3 xaxis(matrix[0], matrix[1], matrix[2]);
			xaxis.Normalize();
			float e = DotProduct(xaxis, delta);
			float f = DotProduct(xaxis, ray_direction);

			if (fabs(f) > 0.001f) {
				float t1 = (e + aabb_min.x) / f;
				float t2 = (e + aabb_max.x) / f;
				if (t1 > t2) { float tmp = t1; t1 = t2; t2 = tmp; }
				if (t2 < Max) Max = t2;
				if (t1 > Min) Min = t1;
				if (Max < Min) return false;
			}
			else {	//	레이가 x축에 거의 수직인상태
				if (-e + aabb_min.x > 0.0f || -e + aabb_max.x < 0.0f)  return false;
			}
		}
		//OOB's Y axis
		{
			vec3 yaxis = vec3(matrix[4], matrix[5], matrix[6]);
			float e = DotProduct(yaxis, delta);
			float f = DotProduct(yaxis, ray_direction);
			if (fabs(f) > 0.001f) {
				float t1 = (e + aabb_min.y) / f;
				float t2 = (e + aabb_max.y) / f;
				if (t1 > t2) { float tmp = t1; t1 = t2; t2 = tmp; }
				if (t2 < Max) Max = t2;
				if (t1 > Min) Min = t1;
				if (Max < Min) return false;
			}
			else {
				if (-e + aabb_min.y > 0.0f || -e + aabb_max.y < 0.0f) return false;
			}
		}
		//OOB's Z axis
		{
			vec3 zaxis(matrix[8], matrix[9], matrix[10]);
			float e = DotProduct(zaxis, delta);
			float f = DotProduct(zaxis, ray_direction);
			if (fabs(f) > 0.001f) {
				float t1 = (e + aabb_min.z) / f;
				float t2 = (e + aabb_max.z) / f;
				if (t1 > t2) { float tmp = t1; t1 = t2; t2 = tmp; }
				if (t2 < Max) Max = t2;
				if (t1 > Min) Min = t1;
				if (Max < Min) return false;
			}
			else {
				if (-e + aabb_min.z > 0.0f || -e + aabb_max.z < 0.0f) return false;
			}
		}

		intersection_distance = Min;
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
	glScalef(2 * extent.x, 2 * extent.y, 2 * extent.z);
	glColor3f(1.0f, 0.0f, 0.0f);
	glutWireCube(1.0f);
	glPopMatrix();
}

Building::Building(vec3 & ext, vec3 & pos, float boundingRadius, float p, float y, float r) : CubeObject::CubeObject(ext, pos, boundingRadius, p, y, r)
{
}

void Building::SetColor(float r, float g, float b)
{
	color.x = r; color.y = g; color.z = b;
}

void Building::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(matrix);
	glColor3f(color.x, color.y, color.z);
	glScalef(2 * extent.x, 2 * extent.y, 2 * extent.z);
	glutSolidCube(1.0f);
	glPopMatrix();
}