#pragma once
#include "BaseEnemy.h"
#include "Transform.h"
#include <array>
#include <memory>

class UnionEnemy;

class UnionBaseEnemy : public BaseEnemy {

private:

	/*===== メンバ変数 =====*/

	// モデル。
	std::shared_ptr<Model> m_model;
	std::shared_ptr<Model> m_modelHit;


	Vec3<float> m_pos;			// 座標
	Vec3<float> m_forwardVec;	// 移動方向ベクトル
	Vec3<float> m_knockBackVec;
	float m_knockBackSpeed;
	const float KNOCK_BACK_SPEED = 2.5f;
	float m_scale;				// 大きさ
	float m_speed;				// 移動速度
	const float SPEED = 0.6f;	// 移動速度のデフォルト値。
	bool m_isActive;			// 生存フラグ
	int m_hitEffectTimer;		// 弾に当たったフラグ
	const int HIT_EFFECT_TIMER = 10;

	int m_hp;
	int m_shotTimer;
	const int SHOT_TIMER = 300;

	// UNIONの敵の周りの敵
	std::array<std::shared_ptr<UnionEnemy>, 6> m_unionEnemy;

	//モデル
	Transform m_transform;

	// スケール
	const float UNION_SCALE = 6.0f;

	// HP
	const int UNION_HP = 5;


	ENEMY_INFO::ID m_id;


public:

	/*===== メンバ関数 =====*/

	UnionBaseEnemy(std::shared_ptr<Model> DefModel, std::shared_ptr<Model> DamageModel);
	void Init()override;
	void Generate(ENEMY_INFO::ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos, const Vec3<float> ForwardVec)override;
	void Update(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize)override;
	void Draw(Camera& Cam)override;
	bool GetIsActive()override { return m_isActive; };
	float GetScale()override { return m_scale; };
	Vec3<float> GetPos()override { return m_pos; };

private:

	// 弾との当たり判定。
	void CheckHitBullet(std::weak_ptr< BulletMgr> BulletMgr, const float& MapSize, const Vec3<float>& PlayerPos);

	// 弾を撃つ処理。
	void Shot(std::weak_ptr< BulletMgr> BulletMgr, const Vec3<float>& PlayerPos);

};