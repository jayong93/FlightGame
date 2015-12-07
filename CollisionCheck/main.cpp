#include "std.h"
#include "Unit.h"
#include "Player.h"
#include "Road.h"
#include "text.h"
#include "StageManager.h"
#include "InputManager.h"
#include <time.h>

static const int width = 1024, height = width/16.0*9;

int GetWindowWidth()
{
	return width;
}

int GetWindowHeight()
{
	return height;
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
		vPositon(px, py, pz), fPitch(fP), fYaw(fY), fRoll(fR), target(nullptr) {}

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

			vec3 cPos = pPos + (up * 20) - (dir * 50);
			vec3 at = cPos + dir;
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

} Camera(0.0f, 60.0f, 0.0f, -10.0f, 0.0f, 0.0f);

std::vector<Object*> objList;


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

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	float lightValue[] = { 1,1,1,1 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightValue);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightValue);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	//float globalAmbient[] = { 0.35, 0.35, 0.35, 1 };
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

	// Object Init
	objList.push_back(new Building(vec3(40, 10, 5), vec3(0, 0, 0), vec3(5, 3, 5).GetSize(), 0.0f, 45.0f, 0.0f));
	((Building*)objList[0])->SetColor(1, 0, 1);
	objList.push_back(new Drone(vec3(0, 0, 0), 5.0f));
	objList.push_back(new Road(0, 0, 3000, 500, 6000, 0, 0, 0));
	objList.push_back(new Player(0, 3, 5000));

	Camera.SetTarget((Player*)objList.back());

	StageManager* stm = StageManager::Instance();
	stm->Init();

	t3dInit();

	glutMainLoop();
}

void Reshape(int w, int h) {
	//ViewPort
	glViewport(0, 0, w, h);

	//Projenction
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//Clip공간 설정
	gluPerspective(60.0, w / float(h), 1.0, 2000.0);

	//ModelView
	glMatrixMode(GL_MODELVIEW);
}

void DrawScene() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();

	glScalef(0.3, 0.3, 0.3);

	// Camera Transform
	Camera.CameraTransform();

	float lightPos[4] = { 0,1,1,0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	BulletManager* bulletmger = BulletManager::Instance();
	bulletmger->Render();

	StageManager* stm = StageManager::Instance();
	stm->Render();

	for (unsigned int i = 0; i < objList.size(); ++i) objList[i]->Render();

	glPopMatrix();

	glutSwapBuffers();
}

void TimerFunction(int value) {
	static clock_t prevClock = 0;
	clock_t nowClock = clock();
	float frameTime = (nowClock - prevClock) / float(CLOCKS_PER_SEC);
	//printf("FPS : %f\n", 1 / frameTime);
	prevClock = nowClock;

	static_cast<Player*>(objList.back())->Update(frameTime);
	glutTimerFunc(5, TimerFunction, 1);
	glutPostRedisplay();
}

void ProcessKeyInput(unsigned char key, int x, int y)
{
	InputManager::GetInstance()->PushKey(key);
}

void ProcessSpeciaKeyInput(int key, int x, int y)
{
	InputManager::GetInstance()->PushKey(key+300);
}

void ProcessKeyRelease(unsigned char key, int x, int y)
{
	InputManager::GetInstance()->ReleaseKey(key);
}

void ProcessSpeciaKeyRelease(int key, int x, int y)
{
	InputManager::GetInstance()->ReleaseKey(key+300);
}

