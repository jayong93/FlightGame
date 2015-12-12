#pragma once
#include "Object.h"
#include "Ring.h"
#include "Unit.h"
#include "Effect.h"
#include <vector>

enum EFFECT { FLAME, SPARK };

class Bullet;
class Player;

class StageManager {
	struct Rect {
		vec3 vPoint1, vPoint2;

		bool isLeafNode;
		std::vector<Building> pObjectList;

		Rect() : vPoint1(), vPoint2(), isLeafNode(false) {	}

		bool Isin(vec3& p1, vec3& p2) {
			if (vPoint1.x < p2.x && p1.x < vPoint2.x
				&&vPoint1.z < p2.z && p1.z < vPoint2.z) {
				return(true);
			}
			return(false);
		}
		void Draw() {
			if (isLeafNode) {
				for (int i = 0; i<pObjectList.size(); ++i) {
					pObjectList[i].Render();
				}
			}
		}
	};
	Rect quadTree[85];

	static StageManager* instance;
	int nodeNum;

	unsigned char** node;
	float mapW, mapH;

	std::vector<Object*> objectList;
	std::vector<Ring*> ringList;
	std::vector<Drone*> droneList;
	std::vector<Effect*> effectList;

	StageManager();
public:
	~StageManager();

	static StageManager* Instance();

	std::vector<std::vector<Building>*> GetBuildingList(float x, float z, float radius);
	std::vector<Drone*>* GetDroneList();

	void Init(Unit*);

	void Render();

	void Update(float frameTime);
	Node GetNearestNode(float x, float z);
	Node GetNearestNode(float x, float z, Node& des);

	int GetItemMaxCount() { return ringList.size(); }
	int GetItemCount();

	unsigned char GetNodeDate(int row, int col);

	bool GetAStarRoute(Node& start, Node& destiny, std::vector<Node>& routeList);

	void CollisionCheck(Player* player);

	void CollisonCheck_Bullet(std::vector<Bullet>* bulletList);

	void CallEffenct(int effectname, const vec3& pos, vec3& color);
};