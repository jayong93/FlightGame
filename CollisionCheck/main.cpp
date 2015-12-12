#include "std.h"
#include "Unit.h"
#include "Player.h"
#include "Road.h"
#include "Ring.h"
#include "text.h"
#include "StageManager.h"
#include "InputManager.h"
#include <time.h>


static const int width = 1024, height = width / 16.0 * 9;
const float mapWidth = 200, mapHeight = 200;
static float fov = 60;

int GetWindowWidth()
{
	return width;
}

int GetWindowHeight()
{
	return height;
}

float* GetFovValue()
{
	return &fov;
}

void DrawScene();
void Reshape(int, int);
void TimerFunction(int);
void ProcessKeyInput(unsigned char, int, int);
void ProcessSpeciaKeyInput(int, int, int);
void ProcessKeyRelease(unsigned char, int, int);
extern void ProcessSpeciaKeyRelease(int, int, int);


class CCamera {
	vec3 vPositon;

	float fPitch;
	float fYaw;
	float fRoll;

	Player* target;

public:
	CCamera(float px = 0.0f, float py = 0.0f, float pz = 300.0f, float fP = 0.0f, float fY = 0.0f, float fR = 0.0f) :
		vPositon(px, py, pz), fPitch(fP), fYaw(fY), fRoll(fR), target(nullptr), isLookBack(false) {}

	bool isLookBack;

	void CameraTransform() {
		glMatrixMode(GL_MODELVIEW);

		if (target)
		{
			float mat[16];
			target->GetMatrix(mat);

			vec3 pPos = target->GetPos();
			vec3 dir(0, 0, -1), up(0, 1, 0);
			dir = dir.MultMatrix(mat);
			up = up.MultMatrix(mat);

			vec3 cPos, at;
			if (isLookBack)
			{
				cPos = pPos + (up * 20) + (dir * 50);
				at = cPos - dir;
			}
			else
			{
				cPos = pPos + (up * 20) - (dir * 50);
				at = cPos + dir;

			}
			gluLookAt(cPos.x, cPos.y, cPos.z, at.x, at.y, at.z, up.x, up.y, up.z);
		}

		else
		{
			glTranslatef(-vPositon.x, -vPositon.y, -vPositon.z);
			glRotatef(-fPitch, 1.0f, 0.0f, 0.0f);
			glRotatef(-fYaw, 0.0f, 1.0f, 0.0f);
			glRotatef(-fRoll, 0.0f, 0.0f, 1.0f);
		}
	}

	void PitchRotate(float fDelta) {
		fPitch += fDelta;
	}

	void RollRoate(float fDelta) {
		fRoll += fDelta;
	}

	void YawRotate(float fDelta) {
		fYaw += fDelta;
	}

	void Move(const vec3& vShift) {
		vPositon.x += vShift.x;
		vPositon.y += vShift.y;
		vPositon.z += vShift.z;
	}

	void CameraInit() {
		vPositon.x = 0.0f;
		vPositon.y = 0.0f;
		vPositon.z = 500.0f;

		fPitch = -30.0f;
		fYaw = 45.0f;
		fRoll = 0.0f;
	}

	void SetTarget(Player* p)
	{
		target = p;
	}
} Camera(0.0f, 100.0f, 500.0f, -10.0f, 0.0f, 0.0f);

std::vector<Object*> objList;
Unit* target;

int main() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Example");

	glutDisplayFunc(DrawScene);
	glutReshapeFunc(Reshape);
	glutTimerFunc(5, TimerFunction, 1);
	glutKeyboardFunc(ProcessKeyInput);
	glutKeyboardUpFunc(ProcessKeyRelease);
	glutSpecialFunc(ProcessSpeciaKeyInput);
	glutSpecialUpFunc(ProcessSpeciaKeyRelease);

	srand(time(NULL));

	// OpenGL Setting
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	float lightValue[] = { 1,1,1,1 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightValue);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightValue);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	float globalAmbient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

	// Object Init
	objList.push_back(new Road(0, 0, 3000, 400, 6000, 0, 0, 0));
	objList.push_back(new Player(0, 3, 100));

	Camera.SetTarget((Player*)objList.back());

	StageManager* stm = StageManager::Instance();
	stm->Init();
	objList.push_back(new Drone((Unit*)objList[1], vec3(0, 5, 0), 12.5f));
	objList.back()->SetDes();

	t3dInit();

	clock();
	glutMainLoop();
}

void Reshape(int w, int h) {
	//ViewPort
	glViewport(0, 0, w, h);

	//Projenction
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//Clip공간 설정
	gluPerspective(fov, w / float(h), 0.01, 8000.0);

	//ModelView
	glMatrixMode(GL_MODELVIEW);
}

void DrawScene() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 메인 뷰
	glViewport(0, 0, width, height);

	//Projenction
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//Clip공간 설정
	gluPerspective(fov, width / float(height), 0.01, 8000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	{
		glScalef(0.5, 0.5, 0.5);

		// Camera Transform
		Camera.CameraTransform();

		float lightPos[4] = { 0,2,1,0 };
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

		BulletManager* bulletmger = BulletManager::Instance();
		bulletmger->Render();

		StageManager* stm = StageManager::Instance();
		stm->Render();

		for (unsigned int i = 0; i < objList.size(); ++i) objList[i]->Render();
	}
	glPopMatrix();

	// 미니맵 뷰
	glViewport(width - mapWidth, 0, mapWidth, mapHeight);

	//Projenction
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//Clip공간 설정
	glOrtho(-mapWidth * 4, mapWidth * 4, -mapHeight * 4, mapHeight * 4, 0.0, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	{
		// 플레이어 위치
		vec3 camPos = objList[1]->GetPos();

		glRotatef(90, 1, 0, 0);
		glTranslatef(-camPos.x, -700, -camPos.z);

		float mapLightPos[4] = { 0,1,0,0 };
		glLightfv(GL_LIGHT0, GL_POSITION, mapLightPos);

		// 검은 배경
		glPushMatrix();
		{
			glTranslatef(0, -10, 0);
			glRotatef(-90, 1, 0, 0);
			glColor3f(0, 0, 0);
			glRectf(-6000, -6000, 6000, 6000);
		}
		glPopMatrix();

		StageManager* stm = StageManager::Instance();
		stm->Render();

		// 플레이어 마크 그리기
		glPushMatrix();
		{
			float mat[16];
			vec3 dir, unit(0, 0, -1);
			objList[1]->GetMatrix(mat);
			dir = unit.MultMatrix(mat);

			dir.y = 0;
			dir.Normalize();

			glTranslatef(camPos.x, 500, camPos.z);
			if (dir.x < 0)
			{
				glRotatef(acosf(dir.Dot(unit)) * 180 / 3.14, 0, 1, 0);
			}
			else
			{
				glRotatef(-acosf(dir.Dot(unit)) * 180 / 3.14, 0, 1, 0);
			}
			glColor3f(1, 0.0, 0.0);
			glNormal3f(0, 1, 0);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0, 0, -40);
				glVertex3f(-40, 0, 40);
				glVertex3f(0, 0, 30);
				glVertex3f(40, 0, 40);
			}
			glEnd();
		}
		glPopMatrix();

		// 드론 마크 그리기
		glPushMatrix();
		{
			vec3 dronePos = objList.back()->GetPos();
			glTranslatef(dronePos.x, 500, dronePos.z);
			glColor3f(1, 1, 1);
			glutSolidSphere(30, 30, 30);
		}
		glPopMatrix();
	}
	glPopMatrix();

	glutSwapBuffers();
}

void TimerFunction(int value) {
	static clock_t prevClock = 0;
	clock_t nowClock = clock();
	float frameTime = (nowClock - prevClock) / float(CLOCKS_PER_SEC);
	//printf("FPS : %f\n", 1 / frameTime);
	prevClock = nowClock;

	BulletManager* bm = BulletManager::Instance();
	bm->Update(frameTime);

	StageManager* sm = StageManager::Instance();
	sm->Update(frameTime);

	for (int i = 0; i < objList.size(); ++i) objList[i]->Update(frameTime);

	glutTimerFunc(16, TimerFunction, 1);
	glutPostRedisplay();
}

void ProcessKeyInput(unsigned char key, int x, int y)
{
	if (key == 'f')
		Camera.isLookBack = true;
	InputManager::GetInstance()->PushKey(key);
}

void ProcessSpeciaKeyInput(int key, int x, int y)
{
	InputManager::GetInstance()->PushKey(key + 300);
}

void ProcessKeyRelease(unsigned char key, int x, int y)
{
	if (key == 'f')
		Camera.isLookBack = false;
	InputManager::GetInstance()->ReleaseKey(key);
}

void ProcessSpeciaKeyRelease(int key, int x, int y)
{
	InputManager::GetInstance()->ReleaseKey(key + 300);
}

