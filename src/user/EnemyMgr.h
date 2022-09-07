#pragma once
#include <array>
#include <memory>
#include "Vec.h"

class Enemy;
class Camera;
class BulletMgr;

class EnemyMgr {

private:

	/*===== メンバ変数 =====*/

	std::array<std::shared_ptr<Enemy>, 64> m_enemy;

	int m_stopEnemyTimer;
	const int STOP_ENEMY_TIMER = 90;
	int m_straightEnemyTimer;
	const int STRAIGHT_ENEMY_TIMER = 60;
	int m_trackingEnemyTimer;
	const int TRACKING_ENEMY_TIMER = 60;


public:

	/*===== メンバ関数 =====*/

	EnemyMgr();
	void Init();
	void Update(std::weak_ptr< BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize);
	void Draw(Camera& NowCam);

	// 一番近くにいる敵の場所をかえす。
	Vec3<float> SearchNearestEnemy(const Vec3<float>& Pos);

	// 敵の近くにいる判定。
	bool CheckEnemyEdge(const Vec3<float>& Pos, const float& Size);

	// 指定の範囲の敵を倒す。
	void AttackEnemy(const Vec3<float>& Pos, const float& Size);


private:

	void Generate(const Vec3<float>& PlayerPos, const int& EnemyID, const float& MapSize);

};