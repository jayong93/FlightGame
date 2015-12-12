#pragma once
#include "std.h"
#include "Vector.h"
#include <vector>
enum FIREWORK { LAUNCH, EXPLODE };
float specular[] = { 1.0f,1.0f,1.0f,1.0f };

struct vec2 {
	int x;
	int y;

	vec2(int x, int y) : x(x), y(y) { }
};

class Fireworks {

	struct BresenhamLine {
		vec2 start;
		vec2 end;
		std::vector<vec2> pointList;

		int maxlen;
		int cdx;
		int clen;
		bool inverse;

		BresenhamLine(vec2& s, vec2& e, int maxl) : start(s), end(e), maxlen(maxl), cdx(0), clen(1), inverse(false) {
			//브레즌함 알고리즘
			int x1 = start.x, y1 = start.y;
			int x2 = end.x, y2 = end.y;

			int dx = abs(x2 - x1), dy = abs(y2 - y1);
			int p;
			int c1, c2;
			int value;


			if (dy <= dx) {
				c1 = 2 * dy;
				c2 = 2 * (dy - dx);

				if (x2 < x1) {
					int tmp;
					tmp = x1; x1 = x2; x2 = tmp;
					tmp = y1; y1 = y2; y2 = tmp;
					inverse = true;
				}

				if (y1 < y2) value = 1;
				else value = -1;


				pointList.push_back(vec2(x1, y1));
				p = 2 * dy - dx;

				for (int x = x1; x < x2; ++x) {
					if (0 > p)	p += c1;
					else { p += c2; y1 += value; }
					pointList.push_back(vec2(x + 1, y1));
				}
			}
			else
			{
				c1 = 2 * dx;
				c2 = 2 * (dx - dy);

				if (y2 < y1) {
					int tmp;
					tmp = y1; y1 = y2; y2 = tmp;
					tmp = x1; x1 = x2; x2 = tmp;
					inverse = true;
				}

				if (x1<x2) value = 1;
				else value = -1;

				pointList.push_back(vec2(x1, y1));

				p = 2 * dx - dy;

				for (int y = y1; y < y2; ++y) {
					if (0 > p) p += c1;
					else { p += c2;	x1 += value; }
					pointList.push_back(vec2(x1, y + 1));
				}
			}
			if (inverse) cdx = pointList.size() - 1;
		}
	};
	vec3 vPosition;
	float pitch, yaw, roll;
	BresenhamLine shellLine;
	std::vector<BresenhamLine> emberLineList;
	vec3 vColor;
	int state;
public:
	Fireworks(vec3& pos, float p, float yaw, float r) : vPosition(pos), pitch(p), yaw(yaw), roll(r)
		, shellLine(vec2(0, 0), vec2(0, rand() % 400 + 300), 20), vColor((rand() % 9) / 10.0f + 0.1f, (rand() % 9) / 10.0f + 0.1f, (rand() % 9) / 10.0f + 0.1f)
		, state(FIREWORK::LAUNCH) {
		int size = rand() % 200 + 100;
		int y = shellLine.end.y;
		float rad = 0.0f;
		for (int i = 0; i < 12; ++i) {
			emberLineList.push_back(BresenhamLine(vec2(0, y), vec2(size*cosf(rad), y + size*sinf(rad)), 20));
			rad += (3.14f) / 6.0f;
		}
	}

	void Init() {
		vColor = vec3((rand() % 9) / 10.0f + 0.1f, (rand() % 9) / 10.0f + 0.1f, (rand() % 9) / 10.0f + 0.1f);
		shellLine.cdx = 0; shellLine.clen = 1;
		for (unsigned int i = 0; i < emberLineList.size(); ++i) {
			if (!emberLineList[i].inverse) emberLineList[i].cdx = 0;
			else emberLineList[i].cdx = emberLineList[i].pointList.size() - 1;
			emberLineList[i].clen = 1;
		}
	}

	void Update() {
		if (state == FIREWORK::LAUNCH) {
			if (shellLine.cdx + 5 < shellLine.pointList.size()) shellLine.cdx += 5;
			if (shellLine.cdx + 5 < shellLine.pointList.size() && shellLine.clen < shellLine.maxlen) ++shellLine.clen;
			else if (shellLine.cdx + 5 >= shellLine.pointList.size()) --shellLine.clen;
			if (shellLine.clen == 0) state = FIREWORK::EXPLODE;
		}
		else if (state == FIREWORK::EXPLODE) {
			bool flag = true;
			for (unsigned int i = 0; i < emberLineList.size(); ++i) {
				if (!emberLineList[i].inverse) {
					if (emberLineList[i].cdx + 2 < emberLineList[i].pointList.size()) emberLineList[i].cdx += 2;
					if (emberLineList[i].cdx + 2 < emberLineList[i].pointList.size() && emberLineList[i].clen < emberLineList[i].maxlen) ++emberLineList[i].clen;
					else if (emberLineList[i].clen > 0 && emberLineList[i].cdx + 2 >= emberLineList[i].pointList.size()) --emberLineList[i].clen;
					if (emberLineList[i].clen <= 0) flag = flag && true;
					else flag = flag && false;
				}
				else {
					if (emberLineList[i].cdx - 2 >= 0) emberLineList[i].cdx -= 2;
					if (emberLineList[i].cdx - 2 >= 0 && emberLineList[i].clen < emberLineList[i].maxlen) ++emberLineList[i].clen;
					else if (emberLineList[i].clen > 0 && emberLineList[i].cdx - 2 < 0) --emberLineList[i].clen;
					if (emberLineList[i].clen <= 0) flag = flag && true;
					else flag = flag && false;
				}
			}
			if (flag == true) {
				Init();
				state = FIREWORK::LAUNCH;
			}
		}
	}

	void Render() {
		glPushMatrix();
		glScalef(7.0f, 7.0f, 7.0f);
		glTranslatef(vPosition.x, vPosition.y, vPosition.z);
		glRotatef(pitch, 1.0f, 0.0f, 0.0f);
		glRotatef(yaw, 0.0f, 1.0f, 0.0f);
		glRotatef(roll, 0.0f, 0.0f, 1.0f);

		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMateriali(GL_FRONT, GL_SHININESS, 100);
		glMatrixMode(GL_MODELVIEW);


		if (state == FIREWORK::LAUNCH) {
			for (int i = 0; i < shellLine.clen; ++i) {
				glColor3f(1.0f, 1.0f, 1.0f);
				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();
				glTranslatef(shellLine.pointList[shellLine.cdx - i].x, shellLine.pointList[shellLine.cdx - i].y, 0.0f);
				glutSolidCube(2.0f);
				glPopMatrix();
			}
		}
		else if (state == FIREWORK::EXPLODE) {
			for (unsigned int k = 0; k < emberLineList.size(); ++k) {
				int s = emberLineList[k].clen;
				for (int i = 0; i < emberLineList[k].clen; ++i) {
					glColor3f(vColor.x + 0.1*(s - i), vColor.y + 0.1*(s - i), vColor.z + 0.1*(s - i));
					glMatrixMode(GL_MODELVIEW);
					glPushMatrix();
					if (!emberLineList[k].inverse) glTranslatef(emberLineList[k].pointList[emberLineList[k].cdx - i].x, emberLineList[k].pointList[emberLineList[k].cdx - i].y, 0.0f);
					else glTranslatef(emberLineList[k].pointList[emberLineList[k].cdx + i].x, emberLineList[k].pointList[emberLineList[k].cdx + i].y, 0.0f);
					glutSolidCube(5.0f);
					glPopMatrix();
				}
			}
		}
		glPopMatrix();
	}

};