#include "EnemyMgr.h"
#include "Enemy.h"
#include "KuroFunc.h"
#include <limits>

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

void EnemyMgr::Update(std::weak_ptr< BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize)
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

		index->Update(BulletMgr, PlayerPos, MapSize);

	}

}

void EnemyMgr::Draw()
{

	/*===== 描画処理 =====*/

	for (auto& index : m_enemy) {

		if (!index->GetIsActive()) continue;

		index->Draw();

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
		if (enemyID == Enemy::ID::STOPPING || enemyID == Enemy::ID::TRACKING) {

			generatePos.x = KuroFunc::GetRand(-MapSize, MapSize);
			generatePos.z = KuroFunc::GetRand(-MapSize, MapSize);

		}

		// 生成する。
		index->Generate(enemyID, PlayerPos, generatePos);

		break;

	}

}

Vec3<float> EnemyMgr::SearchNearestEnemy(const Vec3<float>& Pos) {

	/*===== 一番近くにいる敵の座標を求める =====*/

	float nearestLength = std::numeric_limits<float>().max();
	Vec3<float> nearestPos = Vec3<float>();
	for (auto& index : m_enemy) {

		if (!index->GetIsActive()) continue;

		// 距離を求める。
		float length = Vec3<float>(index->GetPos() - Pos).Length();
		if (nearestLength < length) continue;

		nearestLength = length;
		nearestPos = index->GetPos();

	}

	return nearestPos;

}

bool EnemyMgr::CheckEnemyEdge(const Vec3<float>& Pos, const float& Size) {

	/*===== エッジの判定 =====*/

	for (auto& index : m_enemy) {

		if (!index->GetIsActive()) continue;

		if (index->CheckIsEdge(Pos, Size)) {

			return true;

		}

	}

	return false;

}

void EnemyMgr::AttackEnemy(const Vec3<float>& Pos, const float& Size) {

	/*===== 指定の範囲の敵を倒す =====*/

	for (auto& index : m_enemy) {

		if (!index->GetIsActive()) continue;

		// 敵の座標
		Vec3<float> enemyPos = index->GetPos();
		float enemySize = index->GetScale();

		// 当たり判定
		if (!(Vec3<float>(enemyPos - Pos).Length() <= Size + enemySize)) continue;

		index->Damage();

	}

}