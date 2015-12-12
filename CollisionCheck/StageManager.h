#pragma once
#include "Object.h"
#include "Ring.h"
#include <vector>

class Bullet;
<<<<<<< HEAD
=======
class Player;
>>>>>>> refs/remotes/origin/SoYun

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
<<<<<<< HEAD
=======
	std::vector<Ring*> ringList;
>>>>>>> refs/remotes/origin/SoYun

	StageManager();
public:
	~StageManager();

	static StageManager* Instance();

	std::vector<std::vector<Building>*> GetBuildingList(float x, float z, float radius);

	void Init();

	void Render();

	void Update(float frameTime);
<<<<<<< HEAD

	Node GetNearestNode(float x, float z);

=======

	Node GetNearestNode(float x, float z);

>>>>>>> refs/remotes/origin/SoYun
	Node GetNearestNode(float x, float z, Node& des);

	unsigned char GetNodeDate(int row, int col);

	bool GetAStarRoute(Node& start, Node& destiny, std::vector<Node>& routeList);

<<<<<<< HEAD
=======
	void CollisionCheck(Player* player);

>>>>>>> refs/remotes/origin/SoYun
	void CollisonCheck_Bullet(std::vector<Bullet>* bulletList);
};