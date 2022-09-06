#include "EnemyMgr.h"
#include "Enemy.h"
#include "KuroFunc.h"

EnemyMgr::EnemyMgr()
{

	/*===== コンストラクタ =====*/

	for (auto& index : m_enemy) {

		index = std::make_shared<Enemy>();

	}

	m_stopEnemyTimer = 0;
	m_straightEnemyTimer = 0;
	m_trackingEnemyTimer = 0;

}

void EnemyMgr::Init()
{

	/*===== 初期化処理 =====*/

	for (auto& index : m_enemy) {

		index->Init();

	}

	m_stopEnemyTimer = 0;
	m_straightEnemyTimer = 0;
	m_trackingEnemyTimer = 0;

}

void EnemyMgr::Update(const Vec3<float>& PlayerPos, const float& MapSize)
{

	/*===== 更新処理 =====*/

	// 各生成処理
	++m_stopEnemyTimer;
	++m_straightEnemyTimer;
	++m_trackingEnemyTimer;

	if (STOP_ENEMY_TIMER < m_stopEnemyTimer) {

		m_stopEnemyTimer = 0;

		// 生成。
		Generate(PlayerPos, static_cast<int>(Enemy::ID::STOPPING), MapSize);

	}
	if (STRAIGHT_ENEMY_TIMER < m_straightEnemyTimer) {

		m_straightEnemyTimer = 0;

		// 生成。
		Generate(PlayerPos, static_cast<int>(Enemy::ID::STRAIGHT), MapSize);

	}
	if (TRACKING_ENEMY_TIMER < m_trackingEnemyTimer) {

		m_trackingEnemyTimer = 0;

		// 生成。
		Generate(PlayerPos, static_cast<int>(Enemy::ID::TRACKING), MapSize);

	}


	for (auto& index : m_enemy) {

		if (!index->GetIsActive()) continue;

		index->Update(PlayerPos, MapSize);

	}

}

void EnemyMgr::Draw(Camera& NowCam)
{

	/*===== 描画処理 =====*/

	for (auto& index : m_enemy) {

		if (!index->GetIsActive()) continue;

		index->Draw(NowCam);

	}

}

void EnemyMgr::Generate(const Vec3<float>& PlayerPos, const int& EnemyID, const float& MapSize)
{

	/*===== 生成処理 =====*/

	for (auto& index : m_enemy) {

		if (index->GetIsActive()) continue;

		// 敵のID
		Enemy::ID enemyID = static_cast<Enemy::ID>(EnemyID);

		// 生成位置を決める。
		Vec3<float> generatePos = Vec3<float>();
		if (enemyID == Enemy::ID::STOPPING) {

			generatePos.x = KuroFunc::GetRand(-MapSize, MapSize);
			generatePos.z = KuroFunc::GetRand(-MapSize, MapSize);

		}

		// 生成する。
		index->Generate(enemyID, PlayerPos, generatePos);

		break;

	}

}
