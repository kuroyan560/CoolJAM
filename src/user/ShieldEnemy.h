#pragma once
#include "BaseEnemy.h"
#include "Transform.h"

class ShieldEnemy : public BaseEnemy {

private:

	/*===== メンバ変数 =====*/

	// モデル。
	std::shared_ptr<Model> m_model;
	std::shared_ptr<Model> m_modelHit;


	Vec3<float> m_pos;			// 座標
	Vec3<float> m_forwardVec;	// 移動方向ベクトル
	float m_scale;				// 大きさ
	float m_speed;				// 移動速度
	const float SPEED = 0.6f;	// 移動速度のデフォルト値。
	bool m_isActive;			// 生存フラグ
	int m_hitEffectTimer;		// 弾に当たったフラグ
	const int HIT_EFFECT_TIMER = 10;

	int m_hp;
	int m_shotTimer;
	const int SHOT_TIMER = 300;

	//モデル
	Transform m_transform;

	// スケール
	const float SHIELD_SCALE = 12.0f;

	// HP
	const int SHIELD_HP = 30;

	const float SHIELD_ROT = 0.01f;

	ENEMY_INFO::ID m_id;


public:

	/*===== メンバ関数 =====*/

	ShieldEnemy(std::shared_ptr<Model> DefModel, std::shared_ptr<Model> DamageModel);
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
