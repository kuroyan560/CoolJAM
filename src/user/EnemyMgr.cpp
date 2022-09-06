#include "EnemyMgr.h"
#include "Enemy.h"
#include "KuroFunc.h"
#include <limits>

EnemyMgr::EnemyMgr()
{

	/*===== �R���X�g���N�^ =====*/

	for (auto& index : m_enemy) {

		index = std::make_shared<Enemy>();

	}

	m_stopEnemyTimer = 0;
	m_straightEnemyTimer = 0;
	m_trackingEnemyTimer = 0;

}

void EnemyMgr::Init()
{

	/*===== ���������� =====*/

	for (auto& index : m_enemy) {

		index->Init();

	}

	m_stopEnemyTimer = 0;
	m_straightEnemyTimer = 0;
	m_trackingEnemyTimer = 0;

}

void EnemyMgr::Update(std::weak_ptr< BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize)
{

	/*===== �X�V���� =====*/

	// �e��������
	++m_stopEnemyTimer;
	++m_straightEnemyTimer;
	++m_trackingEnemyTimer;

	if (STOP_ENEMY_TIMER < m_stopEnemyTimer) {

		m_stopEnemyTimer = 0;

		// �����B
		Generate(PlayerPos, static_cast<int>(Enemy::ID::STOPPING), MapSize);

	}
	if (STRAIGHT_ENEMY_TIMER < m_straightEnemyTimer) {

		m_straightEnemyTimer = 0;

		// �����B
		Generate(PlayerPos, static_cast<int>(Enemy::ID::STRAIGHT), MapSize);

	}
	if (TRACKING_ENEMY_TIMER < m_trackingEnemyTimer) {

		m_trackingEnemyTimer = 0;

		// �����B
		Generate(PlayerPos, static_cast<int>(Enemy::ID::TRACKING), MapSize);

	}


	for (auto& index : m_enemy) {

		if (!index->GetIsActive()) continue;

		index->Update(BulletMgr, PlayerPos, MapSize);

	}

}

void EnemyMgr::Draw(Camera& NowCam)
{

	/*===== �`�揈�� =====*/

	for (auto& index : m_enemy) {

		if (!index->GetIsActive()) continue;

		index->Draw(NowCam);

	}

}

void EnemyMgr::Generate(const Vec3<float>& PlayerPos, const int& EnemyID, const float& MapSize)
{

	/*===== �������� =====*/

	for (auto& index : m_enemy) {

		if (index->GetIsActive()) continue;

		// �G��ID
		Enemy::ID enemyID = static_cast<Enemy::ID>(EnemyID);

		// �����ʒu�����߂�B
		Vec3<float> generatePos = Vec3<float>();
		if (enemyID == Enemy::ID::STOPPING || enemyID == Enemy::ID::TRACKING) {

			generatePos.x = KuroFunc::GetRand(-MapSize, MapSize);
			generatePos.z = KuroFunc::GetRand(-MapSize, MapSize);

		}

		// ��������B
		index->Generate(enemyID, PlayerPos, generatePos);

		break;

	}

}

Vec3<float> EnemyMgr::SearchNearestEnemy(const Vec3<float>& Pos) {

	/*===== ��ԋ߂��ɂ���G�̍��W�����߂� =====*/

	float nearestLength = std::numeric_limits<float>().max();
	Vec3<float> nearestPos = Vec3<float>();
	for (auto& index : m_enemy) {

		if (!index->GetIsActive()) continue;

		// ���������߂�B
		float length = Vec3<float>(index->GetPos() - Pos).Length();
		if (nearestLength < length) continue;

		nearestLength = length;
		nearestPos = index->GetPos();

	}

	return nearestPos;

}