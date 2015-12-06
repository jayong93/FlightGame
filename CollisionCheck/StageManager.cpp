#include "StageManager.h"
#include "std.h"
#include <fstream>
#include <json.h>

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
		float height = rand() % 250 + 200;
		Building building(vec3(value["w"].asInt() / 2.0f, height/2.0f, value["h"].asInt()/2.0f),vec3(value["x"].asInt() - mapW / 2, height / 2.0, -(value["y"].asInt() - mapH / 2)), vec3(value["w"].asInt() / 2.0f, height / 2.0f, value["h"].asInt() / 2.0f).GetSize(),0.0f,(float)(rand()%360),0.0f);
		building.SetColor(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
		buildingList.push_back(building);
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
				printf("%3d ", node[i][j]);			// 입력 무결성 확인
			}
			printf("\n");							// 입력 무결성 확인
		}

		fclose(file);
	}
}

void StageManager::Render()
{
	for (unsigned int i = 0; i < buildingList.size(); ++i) buildingList[i].Render();

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
	//		glutSolidSphere(30.0f, 32, 32);
	//		glPopMatrix();
	//	}
	//}
}
