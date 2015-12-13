#include "StageManager.h"
#include "Ring.h"
#include "std.h"
#include "Arm.h"
#include "Player.h"
#include <fstream>
#include <json.h>

#define N 32

//�켱���� ť
class PriorityQueue {
public:
	//���� ������ ������Ʈ ����ü ����
	struct Node
	{
		float value;		//���� �� ����
		int index;		//������ �ε��� �� ����
	};
	//������
	PriorityQueue() : count(0) {}

	//ť�� ������ �ֱ� (v : ���� ��, i : �ε���)
	void Enqueue(float v, int i) {
		int s = count++;		//������ ��ġ
		while (s > 0) {
			int p = (s - 1) / 2; //���� �θ��� �ε���
								 //���� �θ��� ���� �߰��� ������ �۴ٸ� ��������
			if (heap[p].value < v)break;
			heap[s] = heap[p];		//�θ��� ���� �ڽ����� �ű�
			s = p;		//�θ��� ��ġ�� ���ο� ������ġ�� ��
		}
		heap[s].value = v; heap[s].index = i;
	}
	//ť���� ������ �������� (��ȯ�� : �ε���)
	int Dequeue() {
		if (count == 0) return(-1);		//ť�� ������� ���
		int ret = heap[0].index;		//��ȯ�� ����
		int s = 0;						//������ �� ��尪�� ������ ��ġ
		float v = heap[--count].value;	//������ �� ����� �񱳰� ����
		while (2 * s + 1 < count)				//������ ��ġ�� ���� �ڽ��� ���� �ִ� ��
		{
			int min = 2 * s + 1;		//�ڽ��� �ּҰ� �ε����� ���� �ڽ��̶�� ����
										//������ �ڽ��� ���� ���� �ڽİ����� ���� ���
			if (min + 1 < count&&heap[min + 1].value < heap[min].value) ++min;
			//����� ���� �ڽ��� �ּҰ����� ���� ��� ��������
			if (v < heap[min].value) break;
			heap[s] = heap[min];		//�ּҰ��·� ������ ��ġ�� ����
			s = min;					//���ο� ���� ��ġ�� �ּҰ��� ���� �ڽ�����
		}
		heap[s] = heap[count];			//���� ������ ��带 ���� ��ġ�� ����
		return(ret);
	}
private:
	Node heap[N*N];		//������ ����� ���� (Max N*(N-1)/2) - ���� �׷���(symmetric), ������ N*(N-1)
	int count;		//ť�� ����ִ� ������Ʈ(element) ����

};

float heuristic_cost_estimate(int sdx, int ddx) {
	int sRow = sdx / N, sCol = sdx%N;
	int dRow = ddx / N, dCol = ddx%N;
	float h = sqrt((dCol - sCol)*(dCol - sCol) + (dRow - sRow)*(dRow - sRow));
	return h;
}

StageManager* StageManager::instance = nullptr;

StageManager::StageManager() :carTimer(0.0f) {
	quadTree[0].isLeafNode = false;
	quadTree[0].pObjectList.shrink_to_fit();
	quadTree[0].vPoint1 = vec3(-6000.0f, 0.0f, -6000.0f);
	quadTree[0].vPoint2 = vec3(6000.0f, 0.0f, 6000.0f);

	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			quadTree[i * 2 + j + 1].isLeafNode = false;
			quadTree[i * 2 + j + 1].pObjectList.shrink_to_fit();
			quadTree[i * 2 + j + 1].vPoint1 = vec3(-6000.0f + (6000.0f)*j, 0.0f, -6000.0f + (6000.0f)*i);
			quadTree[i * 2 + j + 1].vPoint2 = vec3(0.0f + (6000.0f)*j, 0.0f, 0.0f + (6000.0f)*i);
		}
	}

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			quadTree[i * 4 + j + 5].isLeafNode = false;
			quadTree[i * 4 + j + 5].vPoint1 = vec3(-6000.0f + (3000.0f)*j, 0.0f, -6000.0f + (3000.0f)*i);
			quadTree[i * 4 + j + 5].vPoint2 = vec3(-3000.0f + (3000.0f)*j, 0.0f, -3000.0f + (3000.0f)*i);
		}
	}

	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			quadTree[i * 8 + j + 21].isLeafNode = true;
			quadTree[i * 8 + j + 21].vPoint1 = vec3(-6000.0f + (1500.0f)*j, 0.0f, -6000.0f + (1500.0f)*i);
			quadTree[i * 8 + j + 21].vPoint2 = vec3(-4500.0f + (1500.0f)*j, 0.0f, -4500.0f + (3000.0f)*i);
		}
	}
}

StageManager* StageManager::Instance() {
	if (!instance) instance = new StageManager();
	return instance;
}

std::vector<std::vector<Building>*> StageManager::GetBuildingList(float x, float z, float radius)
{
	std::vector<std::vector<Building>*> ret;
	vec3 p1(x - radius, 0.0f, z - radius);
	vec3 p2(x + radius, 0.0f, z + radius);

	int stack[50];
	int sp = 0;
	stack[sp++] = 0;

	while (sp) {
		int idx = stack[--sp];
		if (quadTree[idx].Isin(p1, p2)) {
			if (quadTree[idx].isLeafNode) {
				ret.push_back(&quadTree[idx].pObjectList);
			}
			else {
				if (idx == 0) {
					for (int i = 4; i >= 1; --i)
						stack[sp++] = 4 * idx + i;
				}
				else if (1 <= idx&&idx <= 4) {
					int sum = 1;
					if (idx % 2 == 0) sum = -1;
					stack[sp++] = 4 * idx + sum;
					stack[sp++] = 4 * idx + sum + 1;
					stack[sp++] = 4 * idx + sum + 4;
					stack[sp++] = 4 * idx + sum + 1 + 4;
				}
				else if (5 <= idx&&idx <= 20) {
					int d = (idx - 1) % 4;
					int sum = 1 - 2 * d;
					stack[sp++] = 4 * idx + sum;
					stack[sp++] = 4 * idx + sum + 1;
					stack[sp++] = 4 * idx + sum + 8;
					stack[sp++] = 4 * idx + sum + 1 + 8;
				}
				else { std::cout << "Error"; exit(0); }
			}
		}
	}
	return ret;
}

std::vector<Drone*>* StageManager::GetDroneList()
{
	return &droneList;
}

StageManager::~StageManager()
{
	if (node)
	{
		for (int i = 0; i < nodeNum; ++i)
		{
			delete[] node[i];
		}
		delete[] node;
	}
}

void StageManager::Init(Unit* target)
{
	std::ifstream ifs("map_data.json", std::ios::in);
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(ifs, root))
		ifs.close();
	Json::Value data = root["data"];
	mapW = root["mw"].asInt();
	mapH = root["mh"].asInt();

	for (int i = 0; i < data.size(); ++i)
	{
		Json::Value& value = data[i];
		if (value["type"].asString() == "building")
		{
			float radius = vec3(value["w"].asInt() / 2.0f, value["d"].asInt() / 2.0f, value["h"].asInt() / 2.0f).GetSize();
			vec3 pos = vec3(value["x"].asInt() - mapW / 2, value["d"].asInt() / 2.0, -(value["y"].asInt() - mapH / 2));
			Building building(vec3(value["w"].asInt() / 2.0f, value["d"].asInt() / 2.0f, value["h"].asInt() / 2.0f), pos, radius, 0.0f, (float)(rand() % 360), 0.0f);
			building.SetColor(rand() / ((float)RAND_MAX * 10 / 7.0) + 0.7f, rand() / ((float)RAND_MAX * 10 / 7.0) + 0.7f, rand() / ((float)RAND_MAX * 10 / 7.0) + 0.7f);

			for (int z = 0; z < 8; ++z) {
				for (int x = 0; x < 8; ++x) {
					if (((pos.x + radius > -6000.0f + (1500.0f)*x) && (pos.x - radius < -4500.0f + (1500.0f)*x))
						&& ((pos.z + radius > -6000.0f + (1500.0f)*z) && (pos.z - radius < -4500.0f + (1500.0f)*z))) {
						quadTree[z * 8 + x + 21].pObjectList.push_back(building);
					}
				}
			}
		}
		else if (value["type"].asString() == "ring")
		{
			// �� ȸ�� �׽�Ʈ
			Ring* ring = new Ring(value["x"].asInt() - mapW / 2, value["z"].asInt(), -(value["y"].asInt() - mapH / 2), value["w"].asInt(), value["d"].asInt(), value["h"].asInt(), value["angle"].asInt(), true);
			ringList.push_back(ring);
		}
	}

	FILE* file;
	if (fopen_s(&file, "map_data_node.txt", "r") == 0)
	{
		fscanf_s(file, "%d", &nodeNum);

		node = new unsigned char*[nodeNum];
		for (int i = 0; i < nodeNum; ++i)
		{
			node[i] = new unsigned char[nodeNum];
		}

		for (int i = 0; i < nodeNum; ++i)
		{
			for (int j = 0; j < nodeNum; ++j)
			{
				int tmp;
				fscanf_s(file, "%d", &tmp);
				node[i][j] = tmp;
			}
		}

		fclose(file);
	}
	else throw "No Map Node Data";

	// �� ����
	int d[8][2] = { { 1, 0 },{ 1, 1 },{ 0, 1 },{ -1, 1 },{ -1, 0 },{ -1, -1 },{ 0, -1 },{ 1, -1 } };

	for (int i = 0; i < 32; ++i) {
		for (int j = 0; j < 32; ++j) {
			for (int k = 0; k < 8; ++k) {
				if (k >= 1 && k <= 4) continue;
				if (node[j][i] & (1 << k)) {
					int nx = d[k][0], nz = d[k][1];
					if (nx + i < 0 || nx + i >= 32 || nz + j < 0 || nz >= 32) continue;
					roadList.push_back(new Road(vec3(387.1*i - 6000, 0.0, 387.1*j - 6000), vec3(387.1f*(nx+i) - 6000, 0.0f, 387.1f*(nz+j) - 6000), 50));
				}
			}
		}
	}

	//	��� ����
	droneList.push_back(new Drone(target, vec3(0, 3.0f, 500.0f), 12.5f));
	droneList.back()->SetDes();
	droneList.push_back(new Drone(target, vec3(-4000.0f, 500.0f, 500.0f), 12.5f));
	droneList.back()->SetDes();
	droneList.push_back(new Drone(target, vec3(3000.0f, 100.0f, 4000.0f), 12.5f));
	droneList.back()->SetDes();
	for (int i = 0; i < 2; ++i) {
		carList.push_back(new Car(vec3(-3000.0f, 3.0f, -3000.0f), 1.0f));
		carList.push_back(new Car(vec3(-3000.0f, 3.0f, 3000.0f), 1.0f));
		carList.push_back(new Car(vec3(0.0f, 3.0f, 0.0f), 1.0f));
		carList.push_back(new Car(vec3(3000.0f, 3.0f, -3000.0f), 1.0f));
		carList.push_back(new Car(vec3(3000.0f, 3.0f, 3000.0f), 1.0f));
	}
}

void StageManager::Restart(Unit* target)
{
	for (auto& d : droneList) delete d;
	droneList.clear();
	droneList.push_back(new Drone(target, vec3(0, 3.0f, 500.0f), 12.5f));
	droneList.back()->SetDes();

	for (auto& r : ringList) r->SetItem(true);
}

void StageManager::Render()
{
	for (int i = 21; i < 85; ++i) quadTree[i].Draw();
	for (auto& r : ringList) r->Render();
	for (auto& d : droneList) d->Render();
	for (auto& e : effectList) e->Render();
	for (auto& road : roadList) road->Render();
	for (auto& c : carList) c->Render();

	//int d[8][2] = { { 1, 0 },{ 1, 1 },{ 0, 1 },{ -1, 1 },{ -1, 0 },{ -1, -1 },{ 0, -1 },{ 1, -1 } };

	//for (int i = 0; i < 32; ++i) {
	//	for (int j = 0; j < 32; ++j) {
	//		glPushMatrix();
	//		glColor3f(1.0f, 0.6f, 0.6f);
	//		glTranslatef((387.1f*i) - 6000, 0.0f, (387.1f*j) - 6000.0f);
	//		for (int k = 0; k < 8; ++k) {
	//			if (node[j][i] & (1 << k)) {
	//				int nx = d[k][0], nz = d[k][1];
	//				if (nx + i < 0 || nx + i >= 32 || nz + j < 0 || nz >= 32) continue;
	//				glBegin(GL_LINES); {
	//					glVertex3f(0.0f, 0.0f, 0.0f);
	//					glVertex3f(387.1f*nx, 0.0f, 387.1f*nz);
	//				}glEnd();
	//			}
	//		}
	//		//glutSolidSphere(30.0f, 32, 32);
	//		glPopMatrix();
	//	}
	//}
}

void StageManager::MinimapRender()
{
	for (int i = 21; i < 85; ++i) quadTree[i].Draw();
	for (auto& r : ringList) r->Render();
}

void StageManager::Update(float frameTime)
{
	for (auto& r : ringList) r->Update(frameTime);
	for (auto& d : droneList) d->Update(frameTime);
	for (auto& e : effectList) e->Update(frameTime);
	for (auto& c : carList) c->Update(frameTime);
	for (auto i = effectList.begin(); i != effectList.end(); ) {
		if (!(*i)->GetAlive()) i = effectList.erase(i);
		else ++i;
	}
	carTimer += frameTime;
	if (carTimer > 0.5f && carList.size()<100) {
		carList.push_back(new Car(vec3(-3000.0f, 3.0f, -3000.0f), 1.0f));
		carList.push_back(new Car(vec3(-3000.0f, 3.0f, 3000.0f), 1.0f)); 
		carList.push_back(new Car(vec3(0.0f, 3.0f, 0.0f), 1.0f));
		carList.push_back(new Car(vec3(3000.0f, 3.0f, -3000.0f), 1.0f));
		carList.push_back(new Car(vec3(3000.0f, 3.0f, 3000.0f), 1.0f));
		carTimer = 0.0f; 
	}
}

Node StageManager::GetNearestNode(float x, float z)
{
	int row = int((z + 6000.0f) / 387.1f + 0.51f);
	int col = int((x + 6000.0f) / 387.1f + 0.51f);
	int d[8][2] = { { 1, 0 },{ 1, 1 },{ 0, 1 },{ -1, 1 },{ -1, 0 },{ -1, -1 },{ 0, -1 },{ 1, -1 } };
	int k = 0;
	int r = row, c = col;
	while ((int)node[r][c] == 0) {
		r = row, c = col;
		r += d[k][1], c += d[k][0];
		k = (k + 1) % 8;
	}
	return Node(r, c);
}

Node StageManager::GetNearestNode(float x, float z, Node & des)
{
	float minH = 1000.0f;
	int d[4][2] = { { 1,0 },{ 0,-1 },{ -1,0 },{ 0,1 } };
	int r = int((z + 6000.0f) / 387.1f);
	int c = int((x + 6000.0f) / 387.1f);
	int row = int((z + 6000.0f) / 387.1f + 0.5f);
	int col = int((x + 6000.0f) / 387.1f + 0.5f);
	int minrow, mincol;
	int s;
	if (c == col) {
		if (r == row) s = 0;
		else s = 3;
	}
	else {
		if (r == row) s = 1;
		else s = 2;
	}
	for (int i = 0; i < 3; ++i) {
		float h = heuristic_cost_estimate(row*N + col, des.row*N + des.col);
		if (h < minH) {
			minH = h;
			minrow = row; mincol = col;
		}
		row = row + d[s][1]; col = col + d[s][0];
		s = ++s % 4;
	}
	return Node(minrow, mincol);
}

int StageManager::GetItemCount()
{
	int count = 0;
	for (auto& r : ringList)
	{
		if (r->IsItemExist())
			count++;
	}
	return count;
}

unsigned char StageManager::GetNodeDate(int row, int col)
{
	return node[row][col];
}

bool StageManager::GetAStarRoute(Node & start, Node & destiny, std::vector<Node>& routeList)
{
	if ((int)node[destiny.row][destiny.col] != 0) {
		struct Vertex
		{
			Vertex() : fscore(0x7fffffff), gscore(0x7fffffff), from(-1), visited(false) { }


			float fscore;	//f�� heuristic �� ���� ������ �ִ�
			int gscore;
			int from;
			bool visited;
		};
		Vertex vertices[N*N];
		PriorityQueue queue;

		vertices[start.row*N + start.col].gscore = 0;
		vertices[start.row*N + start.col].fscore = vertices[start.row*N + start.col].gscore + heuristic_cost_estimate(start.row*N + start.col, destiny.row*N + destiny.col);
		queue.Enqueue(vertices[start.row*N + start.col].fscore, start.row*N + start.col);
		int d[8][2] = { { 1, 0 },{ 1, 1 },{ 0, 1 },{ -1, 1 },{ -1, 0 },{ -1, -1 },{ 0, -1 },{ 1, -1 } };
		while (true) {
			int c = queue.Dequeue();

			if (c == -1) return false;
			if (c == destiny.row*N + destiny.col) {
				routeList.clear();
				while (c != -1) {
					//std::cout << c / N << "  " << c%N << std::endl;
					routeList.push_back(Node(c / N, c % N));
					c = vertices[c].from;
				}
				return true;
			}
			if (vertices[c].visited) continue;
			vertices[c].visited = true;
			int row = c / N, col = c % N;
			//��� ������ ���鿡 ������
			for (int i = 0; i < 8; ++i) {
				if (node[row][col] & (1 << i)) {	//	���� �Ǿ� ������
					int nx = col + d[i][0], ny = row + d[i][1];
					if (nx < 0 || nx >= N || ny < 0 || ny >= N) continue;
					if (vertices[ny*N + nx].visited) continue;
					int tentative_gscore = vertices[c].gscore + 1;
					if (tentative_gscore < vertices[ny*N + nx].gscore) {
						vertices[ny*N + nx].gscore = tentative_gscore;
						vertices[ny*N + nx].from = c;
						vertices[ny*N + nx].fscore = vertices[ny*N + nx].gscore + heuristic_cost_estimate(ny*N + nx, destiny.row*N + destiny.col);
						queue.Enqueue(vertices[ny*N + nx].fscore, ny*N + nx);
					}
				}

			}
		}
		return true;
	}
	return false;
}

void StageManager::CollisionCheck(Player * player)
{
	if (player->GetAlive()) {
		for (auto& r : ringList) player->ColiisionCheck_Ring(r);
		player->CollisionCheck_Building();
		for (auto d : droneList) player->CollisionCheck_Drone(d);
		for (auto i = droneList.begin(); i != droneList.end(); ) {
			if (!(*i)->GetAlive()) i = droneList.erase(i);
			else ++i;
		}
	}
}

void StageManager::CollisonCheck_Bullet(std::vector<Bullet>* bulletList)
{
	//for (unsigned int i = 0; i < bulletList->size(); ++i) {
	//	for (unsigned int j = 0; j < buildingList.size(); ++j)
	//		(*bulletList)[i].CollisionCheck_Building(&buildingList[j]);
	//}
}

void StageManager::CallEffenct(int effectname, const vec3 & pos, vec3& color)
{
	switch (effectname)
	{
	case EFFECT::FLAME: {
		float z = -4.0f;
		while (z <= 4.0f) {
			float raidius = 4.0f*4.0f - z*z;
			for (float radian = 0.0f; radian <= 2 * 3.14f; radian += 0.4) {
				vec3 dir(cosf(radian)*raidius, sinf(radian)*raidius, z);
				dir.Normalize();
				effectList.push_back(new Flame(pos, dir, color));
			}
			z += 0.4f;
		}
		break;
	}
	case EFFECT::SPARK: {
		float z = -4.0f;
		while (z <= 0.0f) {
			float raidius = 4.0f*4.0f - z*z;
			float radian = (rand() % 628) / 100.0f;
			vec3 dir(cosf(radian)*raidius, sinf(radian)*raidius, z);
			dir.Normalize();
			effectList.push_back(new Spark(pos, dir, color, ((rand()%2))/10.0f));
			z += 1.0f;
		}
		break;
	}
	default:
		break;
	}
}
