#pragma once
#include <array>
#include <memory>
#include "Vec.h"

class Enemy;
class Camera;

class EnemyMgr {

private:

	/*===== ƒƒ“ƒo•Ï” =====*/

	std::array<std::shared_ptr<Enemy>, 64> m_enemy;

	int m_stopEnemyTimer;
	const int STOP_ENEMY_TIMER = 180;
	int m_straightEnemyTimer;
	const int STRAIGHT_ENEMY_TIMER = 60;
	int m_trackingEnemyTimer;
	const int TRACKING_ENEMY_TIMER = 240;


public:

	/*===== ƒƒ“ƒoŠÖ” =====*/

	EnemyMgr();
	void Init();
	void Update(const Vec3<float>& PlayerPos, const float& MapSize);
	void Draw(Camera& NowCam);


private:

	void Generate(const Vec3<float>& PlayerPos, const int& EnemyID, const float& MapSize);

};