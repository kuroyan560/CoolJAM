#pragma once
#include "BaseEnemy.h"
#include "Transform.h"

class EnemyHP;

class PressEnemy : public BaseEnemy {

private:

	/*===== メンバ変数 =====*/



	Vec3<float> m_defPos;
	Vec3<float> m_forwardVec;	// 移動方向ベクトル
	Vec3<float> m_knockBackVec;
	float m_knockBackSpeed;
	float m_speed;				// 移動速度
	float m_returnDefPosSpeed;	// 初期位置に戻る際のスピード
	const float SPEED = 0.6f;	// 移動速度のデフォルト値。
	int m_hitEffectTimer;		// 弾に当たったフラグ
	const int HIT_EFFECT_TIMER = 10;
	const float MAX_KNOCK_BACK_SPEED = 3.5f;
	const float ADD_KNOCK_BACK_SPEED = 0.2f;
	int m_returnDefTimer;
	const int RETURN_DEF_TIMER = 120;

	int m_shotTimer;
	const int SHOT_TIMER = 300;

	const float RETURN_DEFPOS_SPEED = 0.2f;

	//モデル
	Transform m_transform;

	// スケール
	const float SCALE = 32.0f;

	// HP
	static const int HP = 60;

	std::array<std::shared_ptr<EnemyHP>, HP> m_hpUI;


public:

	/*===== メンバ関数 =====*/

	PressEnemy(std::shared_ptr<Model> DefModel, std::shared_ptr<Model> DamageModel);
	void Init()override;
	void Generate(ENEMY_INFO::ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos, const Vec3<float> ForwardVec)override;
	void Update(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize)override;
	void Draw()override;

private:

	// 弾との当たり判定。
	void CheckHitBullet(std::weak_ptr< BulletMgr> BulletMgr, const float& MapSize, const Vec3<float>& PlayerPos);

	// 弾を撃つ処理。
	void Shot(std::weak_ptr< BulletMgr> BulletMgr, const Vec3<float>& PlayerPos);

};