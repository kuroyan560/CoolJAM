#pragma once
#include "BaseEnemy.h"
#include "Transform.h"

class EnemyHP;

class ElecMushiEnemy : public BaseEnemy {

private:

	/*===== メンバ変数 =====*/


	Vec3<float> m_forwardVec;	// 移動方向ベクトル
	float m_speed;				// 移動速度
	const float SPEED = 0.6f;	// 移動速度のデフォルト値。
	int m_hitEffectTimer;		// 弾に当たったフラグ
	const int HIT_EFFECT_TIMER = 10;

	float m_sineWaveTimer;
	const float ADD_SINE_WAVE_TIMER = 0.1f;
	const float SINE_WAVE_LENGTH = 10.0f;
	float m_sineWaveLength;
	float m_defLength;

	int m_shotTimer;
	const int SHOT_TIMER = 300;


	// スケール
	const float SCALE = 3.0f;

	// HP
	static const int HP = 15;

	std::array<std::shared_ptr<EnemyHP>, HP> m_hpUI;


public:

	/*===== メンバ関数 =====*/

	ElecMushiEnemy(std::shared_ptr<Model> DefModel, std::shared_ptr<Model> DamageModel);
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