#include "StageManager.h"
#include "Ring.h"
#include "std.h"
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
		while (2 * s + 1<count)				//������ ��ġ�� ���� �ڽ��� ���� �ִ� ��
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
StageManager::StageManager() { }

StageManager* StageManager::Instance() {
	if (!instance) instance = new StageManager();
	return instance;
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

void StageManager::Init()
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
			Building building(vec3(value["w"].asInt() / 2.0f, value["d"].asInt() / 2.0f, value["h"].asInt() / 2.0f), vec3(value["x"].asInt() - mapW / 2, value["d"].asInt() / 2.0, -(value["y"].asInt() - mapH / 2)), vec3(value["w"].asInt() / 2.0f, value["d"].asInt() / 2.0f, value["h"].asInt() / 2.0f).GetSize(), 0.0f, (float)(rand() % 360), 0.0f);
			building.SetColor(rand() / (float)RAND_MAX * 2 + 0.4f, rand() / (float)RAND_MAX * 2 + 0.4f, rand() / (float)RAND_MAX * 2 + 0.4f);
			buildingList.push_back(building);
		}
<<<<<<< HEAD
=======
		else if (value["type"].asString() == "ring")
		{
			Ring* ring = new Ring(value["x"].asInt() - mapW / 2, value["z"].asInt(), -(value["y"].asInt() - mapH / 2), value["w"].asInt(), value["d"].asInt(), value["h"].asInt(),value["angle"].asInt(), true);
			objectList.push_back(ring);
		}
>>>>>>> refs/remotes/origin/JaeYong
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
				fscanf_s(file, "%d", &node[i][j]);
			}
		}

		fclose(file);
	}
	else throw "No Map Node Data";
}

void StageManager::Render()
{
	for (unsigned int i = 0; i < buildingList.size(); ++i) buildingList[i].Render();
	for (auto& o : objectList) o->Render();

	int d[8][2] = { { 1, 0 },{ 1, 1 },{ 0, 1 },{ -1, 1 },{ -1, 0 },{ -1, -1 },{ 0, -1 },{ 1, -1 } };

	for (int i = 0; i < 32; ++i) {
		for (int j = 0; j < 32; ++j) {
			glPushMatrix();
			glColor3f(1.0f, 0.6f, 0.6f);
			glTranslatef((387.1f*i) - 6000, 0.0f, (387.1f*j) - 6000.0f);
			for (int k = 0; k < 8; ++k) {
				if (node[j][i] & (1 << k)) {
					int nx = d[k][0], nz = d[k][1];
					if (nx + i < 0 || nx + i >= 32 || nz + j < 0 || nz >= 32) continue;
					glBegin(GL_LINES); {
						glVertex3f(0.0f, 0.0f, 0.0f);
						glVertex3f(387.1f*nx, 0.0f, 387.1f*nz);
					}glEnd();
				}
			}
			//glutSolidSphere(30.0f, 32, 32);
			glPopMatrix();
		}
	}
}

void StageManager::Update(float frameTime)
{
	for (auto& b : buildingList) b.Update(frameTime);
	for (auto& o : objectList) o->Update(frameTime);
}

Node & StageManager::GetNearestNode(float x, float z)
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

Node & StageManager::GetNearestNode(float x, float z, Node & des)
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

unsigned char StageManager::GetNodeDate(int row, int col)
{
	return node[row][col];
}

<<<<<<< HEAD
	int d[8][2] = { { 1, 0 },{ 1, 1 },{ 0, 1 },{ -1, 1 },{ -1, 0 },{ -1, -1 },{ 0, -1 },{ 1, -1 } };

	for (int i = 0; i < 32; ++i) {
		for (int j = 0; j < 32; ++j) {
			glPushMatrix();
			glColor3f(1.0f, 0.6f, 0.6f);
			glTranslatef((387.1f*i) - 6000, 0.0f, (387.1f*j) - 6000.0f);
			for (int k = 0; k < 8; ++k) {
				if (node[j][i] & (1 << k)) {
					int nx = d[k][0], nz = d[k][1];
					if (nx + i < 0 || nx + i >= 32 || nz + j < 0 || nz >= 32) continue;
					glBegin(GL_LINES); {
						glVertex3f(0.0f, 0.0f, 0.0f);
						glVertex3f(387.1f*nx, 0.0f, 387.1f*nz);
					}glEnd();
				}
			}
			//glutSolidSphere(30.0f, 32, 32);
			glPopMatrix();
		}
	}
}

Node & StageManager::GetNearestNode(float x, float z)
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

Node & StageManager::GetNearestNode(float x, float z, Node & des)
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

=======
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

>>>>>>> refs/remotes/origin/JaeYong

