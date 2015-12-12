#include "AIState.h"
#include "Unit.h"
#include "StageManager.h"
#include <iostream>

Patrol* Patrol::instance = nullptr;

Patrol::Patrol() { }

Patrol * Patrol::Instance()
{
	if (!instance) instance = new Patrol;
	return instance;
}

void Patrol::Update(Drone& drone, float frameTime)
{
	if ((GetDistance(vec3(drone.position.x, 0.0f, drone.position.z), vec3(drone.target->position.x, 0.0f, drone.target->position.z)) < 1500.0f)
		&& (!drone.target->GetIsStelth()) && (drone.target->GetAlive())) {
		drone.ChangeState(Trace::Instance());
		return;
	}
	drone.Move(drone.velocity*(250 * frameTime));
	if (sqrt(((drone.desNode.col * 387.1f - 6000.0f) - drone.position.x)*((drone.desNode.col * 387.1f - 6000.0f) - drone.position.x)
		+ ((drone.desNode.row * 387.1f - 6000.0f) - drone.position.z)*((drone.desNode.row * 387.1f - 6000.0f) - drone.position.z)) < 10.0f) {
		StageManager* stm = StageManager::Instance();
		Node c = drone.desNode;
		unsigned char data = stm->GetNodeDate(c.row, c.col);
		int d[8][2] = { { 1, 0 },{ 1, 1 },{ 0, 1 },{ -1, 1 },{ -1, 0 },{ -1, -1 },{ 0, -1 },{ 1, -1 } };
		int n = rand() % 8;
		while (!(data & (1 << n))) { n = ++n % 8; }
		drone.SetDes(c.row + d[n][1], c.col + d[n][0]);
		drone.SetDirection(drone.velocity);
	}
}

Trace* Trace::instance = nullptr;

Trace::Trace() { }

Trace * Trace::Instance()
{
	if (!instance) instance = new Trace;
	return instance;
}

void Trace::Update(Drone & drone, float frameTime)
{
	if (GetDistance(vec3(drone.position.x, 0.0f, drone.position.z), vec3(drone.target->position.x, 0.0f, drone.target->position.z)) > 2000.0f
		||drone.target->GetIsStelth()||!drone.target->GetAlive()) {
		drone.ChangeState(Patrol::Instance());
		drone.SetDes();
		return;
	}
	if (drone.position.y > 500.0f) {
		drone.velocity = drone.target->position - drone.position;
		drone.velocity.Normalize();
	}
	else {
		if (sqrt(((drone.desNode.col * 387.1f - 6000.0f) - drone.position.x)*((drone.desNode.col * 387.1f - 6000.0f) - drone.position.x)
			+ ((drone.desNode.row * 387.1f - 6000.0f) - drone.position.z)*((drone.desNode.row * 387.1f - 6000.0f) - drone.position.z)) < 15.0f) {
			if (drone.croute > 0)	drone.SetDes(drone.route[drone.croute - 1].row, drone.route[drone.croute - 1].col);
			--drone.croute;
		}
		StageManager* stm = StageManager::Instance();
		Node nDrone = stm->GetNearestNode(drone.position.x, drone.position.z);
		Node nTarget = stm->GetNearestNode(drone.target->position.x, drone.target->position.z);
		//std::cout << (int)stm->GetNodeDate(nTarget.row, nTarget.col) << std::endl;
		if ((drone.prevTargetnode.col != nTarget.col || drone.prevTargetnode.row != nTarget.row)
			&& (nDrone.col != nTarget.col || nDrone.row != nTarget.row)) {
			if (stm->GetAStarRoute(nDrone, nTarget, drone.route)) {
				int n = drone.route.size();
				//if (((drone.route[n - 1].col * 387.1f - 6000.0f - 5.0f <= drone.position.x && drone.position.x <= drone.route[n - 2].col * 387.1f - 6000.0f + 5.0f) ||
				//	(drone.route[n - 2].col * 387.1f - 6000.0f - 5.0f <= drone.position.x && drone.position.x <= drone.route[n - 1].col * 387.1f - 6000.0f + 5.0f))
				//	&& ((drone.route[n - 1].row * 387.1f - 6000.0f - 5.0f <= drone.position.z && drone.position.z <= drone.route[n - 2].row * 387.1f - 6000.0f + 5.0f)
				//		|| (drone.route[n - 2].row * 387.1f - 6000.0f - 5.0f <= drone.position.z && drone.position.z <= drone.route[n - 1].row * 387.1f - 6000.0f + 5.0f))) {
				//	drone.SetDes(drone.route[n - 2].row, drone.route[n - 2].col);
				//	drone.croute = n - 2;
				//}
				if (n >= 2)
				{
					drone.SetDes(drone.route[n - 2].row, drone.route[n - 2].col);
					drone.croute = n - 2;
				}
				else {
					drone.SetDes(drone.route[n - 1].row, drone.route[n - 1].col);
					drone.croute = n - 1;
				}
			}
			else drone.velocity = vec3(0.0f, 0.0f, 0.0f);
		}
		else if ((nDrone.col == nTarget.col && nDrone.row == nTarget.row)) {
			std::vector<std::vector<Building>*> buildingList = stm->GetBuildingList(drone.position.x, drone.position.z, drone.boundingRad);
			bool flag = false;
			for (int n = 0; n < buildingList.size(); ++n) {
				for (int i = 0; i < buildingList[n]->size(); ++i) {
					float distance;
					if ((*buildingList[n])[i].CollisionCheckRay(drone.position, drone.target->position, distance)) {
						flag = true;
					}
				}
			}
			if (!flag) {
				drone.velocity = drone.target->position - drone.position;
				drone.velocity.Normalize();

			}
			else drone.velocity = vec3();
			/*drone.velocity = vec3();*/
		}
		if (drone.polling) drone.Polling();
		drone.prevTargetnode = nTarget;
	}
	drone.Move(drone.velocity * (250 * frameTime));
	//드론 방향 설정 - 벡터로 각 구하기
	if (GetDistance(vec3(drone.position.x, 0.0f, drone.position.z), vec3(drone.target->position.x, 0.0f, drone.target->position.z)) < 700.0f) {
		if (drone.shotTimer == 0) drone.Shot();
		vec3 dir = drone.target->position - drone.position;
		dir.Normalize();
		drone.SetDirection(dir);
	}
	else drone.SetDirection(drone.velocity);
}
