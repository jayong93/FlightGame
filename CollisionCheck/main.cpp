#include "std.h"
#include "Unit.h"
#include "Player.h"
#include "Road.h"
#include "Ring.h"
#include "text.h"
#include "StageManager.h"
#include "InputManager.h"
#include "Fireworks.h"
#include <time.h>
#include <fmod.hpp>

using namespace FMOD;

static const int width = 1024, height = width / 16.0 * 9;
const float mapWidth = 200, mapHeight = 200;
static float fov = 60;
static StageState stageState;
System* fSystem;
Sound* sound[6];
Channel* channel;
Channel* bgmChan;
std::vector<Fireworks> fireworksList;
int fireworkTimer;

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

StageState* GetStageState()
{
	return &stageState;
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

		if (stageState == MAIN)
		{
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
		else if (stageState == INTRO)
		{
			glTranslatef(0, -1800, -9000);
			glRotatef(fYaw, 0, 1, 0);
		}
		else if (stageState == ENDING)
		{
			glTranslatef(0, -1800, -6000);
			glRotatef(fYaw, 0, 1, 0);
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
	glutCreateWindow("City Flight");

	glutDisplayFunc(DrawScene);
	glutReshapeFunc(Reshape);
	glutTimerFunc(5, TimerFunction, 1);
	glutKeyboardFunc(ProcessKeyInput);
	glutKeyboardUpFunc(ProcessKeyRelease);
	glutSpecialFunc(ProcessSpeciaKeyInput);
	glutSpecialUpFunc(ProcessSpeciaKeyRelease);

	// FMOD Init
	System_Create(&fSystem);
	fSystem->init(40, FMOD_INIT_NORMAL, 0);
	fSystem->createStream("background.ogg", FMOD_LOOP_NORMAL | FMOD_2D, 0, &sound[0]);
	fSystem->createSound("explosion.wav", FMOD_2D, 0, &sound[1]);
	fSystem->createSound("item.wav", FMOD_2D, 0, &sound[2]);
	fSystem->createSound("shot.wav", FMOD_2D, 0, &sound[3]);
	fSystem->createSound("stealth_on.wav", FMOD_2D, 0, &sound[4]);
	fSystem->createSound("stealth_off.wav", FMOD_2D, 0, &sound[5]);

	srand(time(NULL));

	// OpenGL Setting
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
	objList.push_back(new Player(0, 3, 100));

	Camera.SetTarget((Player*)objList.front());

	StageManager* stm = StageManager::Instance();
	stm->Init((Unit*)objList.front());

	t3dInit();

	fSystem->playSound(sound[0], NULL, true, &bgmChan);
	bgmChan->setVolume(0.3);
	glutMainLoop();

	fSystem->release();
	fSystem->close();
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
	glEnable(GL_LIGHTING);

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

		float lightPos[4] = { 0,1,0,0 };
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

		BulletManager* bulletmger = BulletManager::Instance();
		bulletmger->Render();

		StageManager* stm = StageManager::Instance();
		stm->Render();
		for (unsigned int i = 0; i < fireworksList.size(); ++i) fireworksList[i].Render();

		if (stageState != ENDING) {
			// 경계 반구 그리기
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(0.7, 0.7, 1.0, 0.5);
			glutSolidSphere(5990, 30, 30);
			glDisable(GL_BLEND);
			glutWireSphere(5990, 30, 30);
		}
		for (unsigned int i = 0; i < objList.size(); ++i) objList[i]->Render();
	}
	glPopMatrix();

	// UI 그리기
	glViewport(0, 0, width, height);
	glDisable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//Clip공간 설정
	glOrtho(0, width, 0, height, -1.0, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	{
		char str[40];
		if (stageState == MAIN)
		{
			sprintf_s(str, 40, "HP : %d", (int)((Player*)objList.front())->GetHp());
			glColor3f(1, 1, 1);
			glRasterPos2f(0, height - 20);
			int len = strlen(str);
			for (int i = 0; i < len; ++i)
			{
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
			}

			sprintf_s(str, 40, "MANA : %d", (int)((Player*)objList.front())->GetMana());
			glRasterPos2f(0, height - 40);
			len = strlen(str);
			for (int i = 0; i < len; ++i)
			{
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
			}

			StageManager* stm = StageManager::Instance();
			sprintf_s(str, 40, "REMAIN ITEM : %d", stm->GetItemCount());
			glRasterPos2f(0, height - 60);
			len = strlen(str);
			for (int i = 0; i < len; ++i)
			{
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
			}
		}
		else if (stageState == INTRO)
		{
			int size = 0, len;
			sprintf_s(str, 40, "PRESS SPACE BAR TO START");
			len = strlen(str);
			for (int i = 0; i < len; ++i)
			{
				size += glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
			}

			glColor3f(1, 1, 1);
			glRasterPos2f(width / 2.0 - size / 2.0, 50);
			for (int i = 0; i < len; ++i)
			{
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
			}
		}
		else if (stageState == ENDING) {

		}
	}
	glPopMatrix();
	if (stageState == MAIN)
	{
		// 미니맵 뷰
		glViewport(width - mapWidth, 0, mapWidth, mapHeight);
		glEnable(GL_LIGHTING);

		//Projenction
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//Clip공간 설정
		glOrtho(-mapWidth * 4, mapWidth * 4, -mapHeight * 4, mapHeight * 4, 0.0, 8000.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glPushMatrix();
		{
			// 플레이어 위치
			vec3 camPos = objList.front()->GetPos();

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
			stm->MinimapRender();

			// 플레이어 마크 그리기
			glPushMatrix();
			{
				float mat[16];
				vec3 dir, unit(0, 0, -1);
				objList.front()->GetMatrix(mat);
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
				StageManager* stm = StageManager::Instance();
				std::vector<Drone*>* dronelist = stm->GetDroneList();
				for (int i = 0; i < dronelist->size(); ++i) {
					vec3 dronePos = (*dronelist)[i]->GetPos();
					glPushMatrix();
					glTranslatef(dronePos.x, 500, dronePos.z);
					glColor3f(1, 1, 1);
					glutSolidSphere(30, 30, 30);
					glPopMatrix();
				}
			}
			glPopMatrix();
		}
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

	fSystem->update();

	if (stageState == INTRO)
	{
		Camera.YawRotate(20 * frameTime);
		IM* im = IM::GetInstance();
		if (im->GetKeyState(' '))
		{
			stageState = MAIN;
			im->ReleaseKey(' ');

			for (auto& o : objList) delete o;
			objList.clear();

			objList.push_back(new Player(0, 3, 100));

			Camera.SetTarget((Player*)objList.front());

			StageManager* sm = StageManager::Instance();
			sm->Restart((Unit*)objList.front());

			bgmChan->setPaused(false);
		}
	}

	else if (stageState == MAIN)
	{
		BulletManager* bm = BulletManager::Instance();
		bm->Update(frameTime);

		StageManager* sm = StageManager::Instance();
		sm->Update(frameTime);

		for (int i = 0; i < objList.size(); ++i) objList[i]->Update(frameTime);

		sm->CollisionCheck((Player*)objList.front());
		bm->CollisionCheck((Player*)objList.front());

		if (sm->GetItemCount() == 0)
		{
			stageState = ENDING;
			bgmChan->setPaused(true);
		}
	}
	else if (stageState == ENDING) {
		Camera.YawRotate(20 * frameTime);
		for (unsigned int i = 0; i < fireworksList.size(); ++i) fireworksList[i].Update();
		++fireworkTimer;
		if (fireworkTimer == 30 && fireworksList.size() < 100) {
			fireworkTimer = 0;
			fireworksList.push_back(Fireworks(vec3(float(rand() % int(2000) - 1000.0f), 0.0f, float(rand() % int(2000) - 1000.0f)), 0.0f, rand() % 360, 0.0f));
		}
	}

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
