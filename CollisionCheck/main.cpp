#include "std.h"
#include "Object.h"

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

public:
	CCamera(float px = 0.0f, float py = 0.0f, float pz = 300.0f, float fP = 0.0f, float fY = 0.0f, float fR = 0.0f) :
		vPositon(px, py, pz), fPitch(fP), fYaw(fY), fRoll(fR) {}

	void CameraTransform() {
		glMatrixMode(GL_MODELVIEW);

		//glTranslatef(-vPositon.x, -vPositon.y, -vPositon.z);

		//glRotatef(-fPitch, 1.0f, 0.0f, 0.0f);
		//glRotatef(-fYaw, 0.0f, 1.0f, 0.0f);
		//glRotatef(-fRoll, 0.0f, 0.0f, 1.0f);

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

} Camera(0.0f, 0.0f, 70.0f, 0.0f, 0.0f, 0.0f);

Building b1(vec3(5, 3, 5), vec3(0, 0, 0), GetDistance(vec3(0.0f, 0.0f, 0.0f), vec3(5, 3, 5)), 0.0f, 0.0f, 0.0f);
Building b2(vec3(5, 3, 5), vec3(0, 10, 0), GetDistance(vec3(0.0f, 0.0f, 0.0f), vec3(5, 3, 5)), 0.0f, 0.0f, 0.0f);

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

	b1.Render();
	b2.Render();
	glPopMatrix();

	glutSwapBuffers();
}

void TimerFunction(int value) {
	b1.Update();
	b2.Update();
	static int i = 0;
	if (b1.CollisionCheck(b2)) std::cout << "OOBB Hit"<< i++ << std::endl;
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
		b1.Rotate(2.0f, 0.0f, 0.0f);
		break;
	case 's':
		b1.Rotate(0.0f, 2.0f, 0.0f);
		break;
	case 'd':
		b1.Rotate(0.0f, 0.0f, 2.0f);
		break;
	case 'w':
		b1.Move(vec3(0.0f, 2.0f, 0.0f));
		break;
	case 'e':
		b1.Move(vec3(0.0f, -2.0f, 0.0f));
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
		b1.Move(vec3(-2.0f, 0.0f, 0.0f));
		break;
	case GLUT_KEY_RIGHT:
		b1.Move(vec3(2.0f, 0.0f, 0.0f));
		break;
	case GLUT_KEY_DOWN:
		b1.Move(vec3(0.0f, 0.0f, 2.0f));
		break;
	case GLUT_KEY_UP:
		b1.Move(vec3(0.0f, 0.0f, -2.0f));
		break;
	default:
		break;
	}
}

