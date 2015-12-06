#include "std.h"
#include "Unit.h"
#include "Player.h"
#include "text.h"

const int width = 800, height = 600;

void DrawScene();
void Reshape(int, int);
void TimerFunction(int);
void ProcessKeyInput(unsigned char, int, int);
void ProcessSpeciaKeyInput(int, int, int);


class CCamera {
	vec3 vPositon;

	float fPitch;
	float fYaw;
	float fRoll;

	Player* target;

public:
	CCamera(float px = 0.0f, float py = 0.0f, float pz = 300.0f, float fP = 0.0f, float fY = 0.0f, float fR = 0.0f) :
		vPositon(px, py, pz), fPitch(fP), fYaw(fY), fRoll(fR) {}

	void CameraTransform() {
		glMatrixMode(GL_MODELVIEW);

		//float mat[16];
		//target->GetMatrix(mat);
		//float tmp;
		//for (int i = 0; i < 3; ++i)
		//{
		//	for (int j = 0; j < 3; ++j)
		//	{
		//		tmp = mat[j * 4 + i];
		//		mat[j * 4 + i] = mat[i * 4 + j];
		//		mat[i * 4 + j] = tmp;
		//	}
		//}
		//for (int i = 0; i < 3; ++i)
		//{
		//	mat[12 + i] = 0;
		//}

		//vec3 pPos = target->GetPos();
		//glTranslatef(-pPos.x, -pPos.y, -pPos.z);

		//glMultMatrixf(mat);
		//glTranslatef(0, 0, -20);

		//glTranslatef(-vPositon.x, -vPositon.y, -vPositon.z);

		//glRotatef(-fPitch, 1.0f, 0.0f, 0.0f);
		//glRotatef(-fYaw, 0.0f, 1.0f, 0.0f);
		//glRotatef(-fRoll, 0.0f, 0.0f, 1.0f);

		float mat[16];
		target->GetMatrix(mat);

		vec3 pPos = target->GetPos();
		vec3 dir(0, 0, -1), up(0, 1, 0);
		dir = dir.MultMatrix(mat);
		up = up.MultMatrix(mat);

		vec3 cPos = pPos + (up * 20) - (dir * 60);
		vec3 at = cPos + dir;
		gluLookAt(cPos.x, cPos.y, cPos.z, at.x, at.y, at.z, up.x, up.y, up.z);
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

} Camera(0.0f, 0.0f, 70.0f, 0.0f, 0.0f, 0.0f);

std::vector<Object*> objList;


int main() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Example");

	glutDisplayFunc(DrawScene);
	glutReshapeFunc(Reshape);
	glutTimerFunc(17, TimerFunction, 1);
	glutKeyboardFunc(ProcessKeyInput);
	glutSpecialFunc(ProcessSpeciaKeyInput);

	objList.push_back(new Building(vec3(5, 3, 5), vec3(0, 10, 0), GetDistance(vec3(0.0f, 0.0f, 0.0f), vec3(5, 3, 5)), 0.0f, 0.0f, 0.0f));
	objList.push_back(new Drone(vec3(0, 0, 0)));
	objList.push_back(new Player(0, 0, 100));

	Camera.SetTarget((Player*)objList[2]);

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
	//Clip���� ����
	gluPerspective(60.0, w / h, 1.0, 1000.0);

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

	for (unsigned int i = 0; i < objList.size(); ++i)
	{
		objList[i]->Render();
	}
	glPopMatrix();

	glutSwapBuffers();
}

void TimerFunction(int value) {
	//b2.Update();
	//static int i = 0;
	//if (objList[0]->CollisionCheck(*objList[1])) std::cout << "OOBB Hit"<< i++ << std::endl;
	static_cast<Player*>(objList[2])->Update();
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
	float mat[16], tmp[16];
	switch (key)
	{
		//case GLUT_KEY_LEFT:
		//	objList[1]->Move(vec3(-2.0f, 0.0f, 0.0f));
		//	break;
		//case GLUT_KEY_RIGHT:
		//	objList[1]->Move(vec3(2.0f, 0.0f, 0.0f));
		//	break;
		//case GLUT_KEY_DOWN:
		//	objList[1]->Move(vec3(0.0f, 0.0f, 2.0f));
		//	break;
		//case GLUT_KEY_UP:
		//	objList[1]->Move(vec3(0.0f, 0.0f, -2.0f));
		//	break;
		//default:
		//	break;
	case GLUT_KEY_LEFT:
		objList[2]->Rotate(0, 0, 10);
		break;
	case GLUT_KEY_RIGHT:
		objList[2]->Rotate(0, 0, -10);
		break;
	case GLUT_KEY_DOWN:
		objList[2]->Rotate(10, 0, 0);
		break;
	case GLUT_KEY_UP:
		objList[2]->Rotate(-10, 0, 0);
		break;
	default:
		break;
	}
}

