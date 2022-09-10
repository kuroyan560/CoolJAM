#include "EnemyMgr.h"
#include "KuroFunc.h"
#include <limits>
#include "PlayerStraightEnemy.h"
#include "PressEnemy.h"
#include "StraightEnemy.h"
#include "ShieldEnemy.h"
#include "StoppingEnemy.h"
#include "TrackingEnemy.h"
#include "TorusMoveEnemy.h"
#include "UnionBaseEnemy.h"
#include "ElecMushiEnemy.h"
#include "../engine/Importer.h"

EnemyMgr::EnemyMgr()
{

	/*===== コンストラクタ =====*/

	m_model = Importer::Instance()->LoadModel("resource/user/", "enemy.glb");
	m_modelHit = Importer::Instance()->LoadModel("resource/user/", "enemy_hit.glb");
	m_elecMushi = Importer::Instance()->LoadModel("resource/user/", "enemy_elecMushi.glb");

	//for (auto& index : m_enemy) {

		//index = std::make_shared<Enemy>();

	//}

}

void EnemyMgr::Init()
{

	/*===== 初期化処理 =====*/

	for (auto& index : m_enemy) {

		// 生成されていなかったら処理を飛ばす。
		if (!index.operator bool()) continue;

		index->Init();

	}

}

void EnemyMgr::Update(std::weak_ptr< BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize)
{

	/*===== 更新処理 =====*/

	for (auto& index : m_enemy) {

		// 生成されていなかったら処理を飛ばす。
		if (!index.operator bool()) continue;

		if (!index->m_isActive) continue;

		index->Update(BulletMgr, PlayerPos, MapSize);

	}

}

void EnemyMgr::Draw()
{

	/*===== 描画処理 =====*/

	for (auto& index : m_enemy) {

		// 生成されていなかったら処理を飛ばす。
		if (!index.operator bool()) continue;

		if (!index->m_isActive) continue;

		index->Draw();

	}

}

void EnemyMgr::Generate(const Vec3<float>& PlayerPos, const Vec3<float>& GeneratePos, const Vec3<float> ForwardVec, const int& EnemyID, const float& MapSize)
{

	/*===== 生成処理 =====*/

	for (auto& index : m_enemy) {

		// 生成されていなかったら
		if (!index.operator bool()) {

			GenerateEnemy(index, PlayerPos, GeneratePos, ForwardVec, EnemyID, MapSize);

			break;

		}

		if (index->m_isActive) continue;

		GenerateEnemy(index, PlayerPos, GeneratePos, ForwardVec, EnemyID, MapSize);

		break;

	}

}

void EnemyMgr::GenerateEnemy(std::shared_ptr<BaseEnemy>& Enemy, const Vec3<float>& PlayerPos, const Vec3<float>& GeneratePos, const Vec3<float> ForwardVec, const int& EnemyID, const float& MapSize)
{

	/*===== 敵を生成する ======*/

	// Enemyの中身が入っていたらリセットを掛ける。
	if (Enemy.operator bool()) {
		Enemy.reset();
	}

	ENEMY_INFO::ID enemyID = static_cast<ENEMY_INFO::ID>(EnemyID);
	switch (enemyID)
	{
	case ENEMY_INFO::ID::STOPPING:
		Enemy = std::make_shared<StoppingEnemy>(m_model, m_modelHit);
		break;
	case ENEMY_INFO::ID::STRAIGHT:
		Enemy = std::make_shared<StraightEnemy>(m_model, m_modelHit);
		break;
	case ENEMY_INFO::ID::PLAYER_STRAIGHT:
		Enemy = std::make_shared<PlayerStraightEnemy>(m_model, m_modelHit);
		break;
	case ENEMY_INFO::ID::TRACKING:
		Enemy = std::make_shared<TrackingEnemy>(m_model, m_modelHit);
		break;
	case ENEMY_INFO::ID::SHIELD:
		Enemy = std::make_shared<ShieldEnemy>(m_model, m_modelHit);
		break;
	case ENEMY_INFO::ID::UNION:
		Enemy = std::make_shared<UnionBaseEnemy>(m_model, m_modelHit);
		break;
	case ENEMY_INFO::ID::TORUS_MOVE:
		Enemy = std::make_shared<TorusMoveEnemy>(m_model, m_modelHit);
		break;
	case ENEMY_INFO::ID::PRESS:
		Enemy = std::make_shared<PressEnemy>(m_model, m_modelHit);
		break;
	case ENEMY_INFO::ID::DASH:
		//Enemy = std::make_shared<DATABITS_16>(m_model, m_modelHit);
		break;
	case ENEMY_INFO::ID::ELEC_MUSHI:
		Enemy = std::make_shared<ElecMushiEnemy>(m_elecMushi, m_modelHit);
		break;
	default:
		break;
	}

	// 生成する。
	Enemy->Generate(enemyID, PlayerPos, GeneratePos, ForwardVec);

}

Vec3<float> EnemyMgr::SearchNearestEnemy(const Vec3<float>& Pos) {

	/*===== 一番近くにいる敵の座標を求める =====*/

	float nearestLength = std::numeric_limits<float>().max();
	Vec3<float> nearestPos = Vec3<float>(-1, -1, -1);
	for (auto& index : m_enemy) {

		if (!index->m_isActive) continue;

		// 距離を求める。
		float length = Vec3<float>(index->m_pos - Pos).Length();
		if (nearestLength < length) continue;

		nearestLength = length;
		nearestPos = index->m_pos;

	}

	return nearestPos;

}

//bool EnemyMgr::CheckEnemyEdge(const Vec3<float>& Pos, const float& Size) {
//
//	/*===== エッジの判定 =====*/
//
//	for (auto& index : m_enemy) {
//
//		if (!index->GetIsActive()) continue;
//
//		if (index->CheckIsEdge(Pos, Size)) {
//
//			return true;
//
//		}
//
//	}
//
//	return false;
//
//}

bool EnemyMgr::CheckHitEnemy(const Vec3<float>& Pos, const float& Size)
{

	/*===== 敵とのあたり判定 =====*/

	bool isHit = false;
	for (auto& index : m_enemy) {

		// 生成されていなかったら処理を飛ばす。
		if (!index.operator bool()) continue;

		if (!index->m_isActive) continue;

		// 二点間の距離
		float length = Vec3<float>(Pos - index->m_pos).Length();

		if (!(length < Size + index->m_scale)) continue;

		isHit = true;
		index->Init();

	}

	return isHit;

}

void EnemyMgr::AttackEnemy(const Vec3<float>& Pos, const float& Size) {

	/*===== 指定の範囲の敵を倒す =====*/

	for (auto& index : m_enemy) {

		// 生成されていなかったら処理を飛ばす。
		if (!index.operator bool()) continue;

		if (!index->m_isActive) continue;

		// 敵の座標
		Vec3<float> enemyPos = index->m_pos;
		float enemySize = index->m_scale;

		// 当たり判定
		if (!(Vec3<float>(enemyPos - Pos).Length() <= Size + enemySize)) continue;

		index->Damage(1);

	}

}