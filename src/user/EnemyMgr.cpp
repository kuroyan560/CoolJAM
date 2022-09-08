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

}

void EnemyMgr::Init()
{

	/*===== 初期化処理 =====*/

	for (auto& index : m_enemy) {

		index->Init();


	}

}

void EnemyMgr::Update(std::weak_ptr< BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize)
{

	/*===== 更新処理 =====*/

	for (auto& index : m_enemy) {

		if (!index->GetIsActive()) continue;

		index->Update(BulletMgr, PlayerPos, MapSize);

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

void EnemyMgr::Generate(const Vec3<float>& PlayerPos, const Vec3<float>& GeneratePos, const Vec3<float> ForwardVec, const int& EnemyID, const float& MapSize)
{

	/*===== 生成処理 =====*/

	for (auto& index : m_enemy) {

		if (index->GetIsActive()) continue;

		// 敵のID
		ENEMY_INFO::ID enemyID = static_cast<ENEMY_INFO::ID>(EnemyID);

		// 生成する。
		index->Generate(enemyID, PlayerPos, GeneratePos, ForwardVec);

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

bool EnemyMgr::CheckHitEnemy(const Vec3<float>& Pos, const float& Size)
{

	/*===== 敵とのあたり判定 =====*/

	bool isHit = false;
	for (auto& index : m_enemy) {

		if (!index->GetIsActive()) continue;

		if (!index->CheckHit(Pos, Size)) continue;

		isHit = true;
		index->Init();

	}

	return isHit;

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

		index->Init();

	}

}