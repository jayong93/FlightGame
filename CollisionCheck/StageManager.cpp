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

	for (size_t i = 0; i < data.size(); ++i)
	{
		Json::Value& value = data[i];
		if (value["type"].asString() == "building")
		{
			Building building(vec3(value["w"].asInt() / 2.0f, value["d"].asInt() / 2.0f, value["h"].asInt() / 2.0f), vec3(value["x"].asInt() - mapW / 2, value["d"].asInt() / 2.0, -(value["y"].asInt() - mapH / 2)), vec3(value["w"].asInt() / 2.0f, value["d"].asInt() / 2.0f, value["h"].asInt() / 2.0f).GetSize(), 0.0f, (float)(rand() % 360), 0.0f);
			building.SetColor(rand() / (float)RAND_MAX * 2 + 0.4f, rand() / (float)RAND_MAX * 2 + 0.4f, rand() / (float)RAND_MAX * 2 + 0.4f);
			buildingList.push_back(building);
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
				int d;
				fscanf_s(file, "%d", &d);
				node[i][j] = d;
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
}
