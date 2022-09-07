#pragma once
#include <array>
#include <memory>
#include "Vec.h"

class Enemy;
class Camera;
class BulletMgr;

class EnemyMgr {

private:

	/*===== �����o�ϐ� =====*/

	std::array<std::shared_ptr<Enemy>, 64> m_enemy;

	int m_stopEnemyTimer;
	const int STOP_ENEMY_TIMER = 90;
	int m_straightEnemyTimer;
	const int STRAIGHT_ENEMY_TIMER = 60;
	int m_trackingEnemyTimer;
	const int TRACKING_ENEMY_TIMER = 60;


public:

	/*===== �����o�֐� =====*/

	EnemyMgr();
	void Init();
	void Update(std::weak_ptr< BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize);
	void Draw(Camera& NowCam);

	// ��ԋ߂��ɂ���G�̏ꏊ���������B
	Vec3<float> SearchNearestEnemy(const Vec3<float>& Pos);

	// �G�̋߂��ɂ��锻��B
	bool CheckEnemyEdge(const Vec3<float>& Pos, const float& Size);


private:

	void Generate(const Vec3<float>& PlayerPos, const int& EnemyID, const float& MapSize);

};