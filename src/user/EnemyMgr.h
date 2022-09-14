
#pragma once
#include <array>
#include <memory>
#include "Vec.h"
#include"EnemyDeadEmitter.h"

class BaseEnemy;
class Camera;
class BulletMgr;
class Model;

class EnemyMgr {

private:

	/*===== メンバ変数 =====*/

	std::array<std::shared_ptr<BaseEnemy>, 64> m_enemy;
	std::array<EnemyDeadEmitter, 64> m_deadEffectEmitterArray;
	std::array<bool, 64> m_initDeadEffectArray;

	std::shared_ptr<Model> m_model;
	std::shared_ptr<Model> m_modelHit;
	std::shared_ptr<Model> m_elecMushi;
	std::shared_ptr<Model> m_elecMushiHit;
	std::shared_ptr<Model>m_coinModel;

	Color m_particleColor;
public:

	/*===== メンバ関数 =====*/

	EnemyMgr();
	void Init();
	void Update(std::weak_ptr< BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize);
	void Draw(Camera& NowCam, std::weak_ptr<RenderTarget>Main, std::weak_ptr<RenderTarget>EmmisiveMap, std::weak_ptr<DepthStencil>DepthStencil);

	//退散
	void AllDisappear();

	// 一番近くにいる敵の場所をかえす。
	Vec3<float> SearchNearestEnemy(const Vec3<float>& Pos);

	// 指定のベクトルに一番近い敵を探す。
	Vec3<float> SearchNearestEnemyToVector(const Vec3<float>& Pos, const Vec3<float>& Vec, const float& ReceivingRate);

	// 敵の近くにいる判定。
	//bool CheckEnemyEdge(const Vec3<float>& Pos, const float& Size);

	// 敵との衝突判定。
	bool CheckHitEnemy(const Vec3<float>& Pos, const float& Size);

	// コインとの衝突判定
	bool CheckHitCoin(const Vec3<float>& Pos, const float& Size, std::weak_ptr< BulletMgr> BulletMgr);

	// 指定の範囲の敵を倒す。
	int AttackEnemy(const Vec3<float>& Pos, const float& Size, std::weak_ptr<BulletMgr> BulletMgr);

	// 敵を生成。
	void Generate(const Vec3<float>& PlayerPos, const Vec3<float>& GeneratePos, const Vec3<float> ForwardVec, const int& EnemyID, const int& ShotTimer, const float& MapSize);

	// すべての敵が死んでいるか。
	bool GetAllEnemyDead();

	// すべての敵を殺す。
	void AllKill(std::weak_ptr<BulletMgr> BulletMgr);

	Color GetParticleColor();
private:

	// 敵生成関数 Generate内で使用する。
	void GenerateEnemy(std::shared_ptr<BaseEnemy>& Enemy, const Vec3<float>& PlayerPos, const Vec3<float>& GeneratePos, const Vec3<float> ForwardVec, const int& EnemyID, const int& ShotTimer, const float& MapSize);

};