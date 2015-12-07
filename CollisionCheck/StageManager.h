#pragma once
#include "Object.h"
#include <vector>


class StageManager {
	static StageManager* instance;
	int nodeNum;

	unsigned char** node;
	float mapW, mapH;

	std::vector<Building> buildingList;

	StageManager();
public:
	~StageManager();

	static StageManager* Instance();

	std::vector<Building>* GetBuildingList() { return(&buildingList); }

	void Init();

	void Render();

	Node& GetNearestNode(float x, float z);

	Node& GetNearestNode(float x, float z, Node& des);

	unsigned char GetNodeDate(int row, int col);

	bool GetAStarRoute(Node& start, Node& destiny, std::vector<Node>& routeList);
};