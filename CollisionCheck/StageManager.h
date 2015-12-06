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

	void Init();

	void Render();
};