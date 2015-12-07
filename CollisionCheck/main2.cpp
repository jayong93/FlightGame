#include "std.h"
#include "Unit.h"
#include "text.h"
#include "StageManager.h"

const int width = 1000, height = 700;
const int size = 12000;

void DrawScene();
void Reshape(int, int);
void TimerFunction(int);
void ProcessKeyInput(unsigned char, int, int);
void ProcessSpeciaKeyInput(int, int, int);


//class CCamera {
//	vec3 vPositon;
//
//	float fPitch;
//	float fYaw;
//	float fRoll;
//
//public:
//	CCamera(float px = 0.0f, float py = 0.0f, float pz = 300.0f, float fP = 0.0f, float fY = 0.0f, float fR = 0.0f) :
//		vPositon(px, py, pz), fPitch(fP), fYaw(fY), fRoll(fR) {}
//
//	void CameraTransform() {
//		glMatrixMode(GL_MODELVIEW);
//
//
//		glTranslatef(-vPositon.x, -vPositon.y, -vPositon.z);
//
//		glRotatef(-fPitch, 1.0f, 0.0f, 0.0f);
//		glRotatef(-fYaw, 0.0f, 1.0f, 0.0f);
//		glRotatef(-fRoll, 0.0f, 0.0f, 1.0f);
//
//	}
//
//	void PitchRotate(float fDelta) {
//		fPitch += fDelta;
//	}
//
//	void RollRoate(float fDelta) {
//		fRoll += fDelta;
//	}
//
//	void YawRotate(float fDelta) {
//		fYaw += fDelta;
//	}
//
//	void Move(const vec3& vShift) {
//		vPositon.x += vShift.x;
//		vPositon.y += vShift.y;
//		vPositon.z += vShift.z;
//	}
//
//	void CameraInit() {
//		vPositon.x = 0.0f;
//		vPositon.y = 0.0f;
//		vPositon.z = 500.0f;
//
//		fPitch = -30.0f;
//		fYaw = 45.0f;
//		fRoll = 0.0f;
//	}
//
//} Camera(0.0f, 0.0f, 70.0f, 0.0f, 0.0f, 0.0f);
class CCamera {
	vec3 vPositon;

	float fPitch;
	float fYaw;
	float fRoll;

public:
	CCamera(float px = 0.0f, float py = 0.0f, float pz = 300.0f, float fP = 0.0f, float fY = 0.0f, float fR = 0.0f) :
		vPositon(px, py, pz), fPitch(fP), fYaw(fY), fRoll(fR) {}

	void CameraTransform() {
		glMatrixMode(GL_MODELVIEW);


		glTranslatef(-vPositon.x, -vPositon.y, -vPositon.z);
		glTranslatef(0.0f, vPositon.y, 0.0f);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	}

	void PitchRotate(float fDelta) {
		fPitch += fDelta;
	}

	void RollRoate(float fDelta) {
		fRoll += fDelta;
	}

	void YawRotate(float fDelta) {
		fYaw += fDelta;
		if (fYaw > 360.0f) fYaw -= 360.0f;
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

	vec3 GetCameraPos() { return(vPositon); }

} Camera(0.0f, 200.0f, 1000.0f, -7.0f, 0.0f, 0.0f);

std::vector<Object*> objList;


int main() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(10, 10);
	glutInitWindowSize(width, height);
	glutCreateWindow("Example");

	glutDisplayFunc(DrawScene);
	glutReshapeFunc(Reshape);
	glutTimerFunc(17, TimerFunction, 1);
	glutKeyboardFunc(ProcessKeyInput);
	glutSpecialFunc(ProcessSpeciaKeyInput);

	objList.push_back(new Building(vec3(5, 3, 5), vec3(0, 10, 0), GetDistance(vec3(0.0f, 0.0f, 0.0f), vec3(5, 3, 5)), 0.0f, 0.0f, 0.0f));
	objList.push_back(new Drone(vec3(0, 0, 0), 5.0f));
	StageManager* stm = StageManager::Instance();
	stm->Init();

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
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
	glOrtho((-size / 2.0f), (size / 2.0f), (-size / 2.0f), (size / 2.0f), 10.0f, 10000.0f);

	//ModelView
	glMatrixMode(GL_MODELVIEW);
}

void DrawScene() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	//	CameraTransform
	Camera.CameraTransform();

	for (unsigned int i = 0; i < objList.size(); ++i)objList[i]->Render();
	BulletManager* bulletmger = BulletManager::Instance();
	bulletmger->Render();
	
	StageManager* stm = StageManager::Instance();
	stm->Render();
	glPopMatrix();
	glutSwapBuffers();
}

void TimerFunction(int value) {
	//b2.Update();
	//static int i = 0;
	//if (i++ % 30 == 0) 
	//	objList[1]->Update();
	//if (objList[0]->CollisionCheck(*objList[1])) std::cout << "OOBB Hit"<< i++ << std::endl;
	BulletManager* bulletmger = BulletManager::Instance();
	bulletmger->Update();
	glutTimerFunc(17, TimerFunction, 1);
	glutPostRedisplay();
}

void ProcessKeyInput(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'y':
		Camera.YawRotate(2.0f);
		break;
	case 'Y':
		Camera.YawRotate(-2.0f);
		break;
	case 'x':
		Camera.PitchRotate(2.0f);
		break;
	case 'X':
		Camera.PitchRotate(-2.0f);
		break;
	case 'z': {
		vec3 vShift;
		vShift.z = -10.0f;
		Camera.Move(vShift);;
		break;
	}
	case 'Z':
	{
		vec3 vShift;
		vShift.z = 10.0f;
		Camera.Move(vShift);;
		break;
	}

	case 'a':
		objList[1]->Rotate(2.0f, 0.0f, 0.0f);
		break;
	case 's':
		objList[1]->Rotate(0.0f, 2.0f, 0.0f);
		break;
	case 'd':
		objList[1]->Rotate(0.0f, 0.0f, 2.0f);
		break;
	case 'w':
		objList[1]->Move(vec3(0.0f, 2.0f, 0.0f));
		break;
	case 'e':
		objList[1]->Move(vec3(0.0f, -2.0f, 0.0f));
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

void ProcessSpeciaKeyInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		objList[1]->Move(vec3(-2.0f, 0.0f, 0.0f));
		break;
	case GLUT_KEY_RIGHT:
		objList[1]->Move(vec3(2.0f, 0.0f, 0.0f));
		break;
	case GLUT_KEY_DOWN:
		objList[1]->Move(vec3(0.0f, 0.0f, 2.0f));
		break;
	case GLUT_KEY_UP:
		objList[1]->Move(vec3(0.0f, 0.0f, -2.0f));
		break;
	default:
		break;
	}
}

