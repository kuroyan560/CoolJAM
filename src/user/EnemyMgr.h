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

	// �w��͈̔͂̓G��|���B
	void AttackEnemy(const Vec3<float>& Pos, const float& Size);

	// �G�𐶐��B
	void Generate(const Vec3<float>& PlayerPos, const Vec3<float>& GeneratePos, const Vec3<float> ForwardVec, const int& EnemyID, const float& MapSize);


private:

};