#include "EnemyMgr.h"
#include "Enemy.h"
#include "KuroFunc.h"

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

void EnemyMgr::Update(const Vec3<float>& PlayerPos, const float& MapSize)
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

		index->Update(PlayerPos, MapSize);

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
		if (enemyID == Enemy::ID::STOPPING) {

			generatePos.x = KuroFunc::GetRand(-MapSize, MapSize);
			generatePos.z = KuroFunc::GetRand(-MapSize, MapSize);

		}

		// ��������B
		index->Generate(enemyID, PlayerPos, generatePos);

		break;

	}

}
