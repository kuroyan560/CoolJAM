#pragma once
#include "Vec.h"
#include "Transform.h"
#include "EnemyWave.h"
#include <memory>
#include <array>

class Model;
class Camera;
class BulletMgr;

class Enemy {

	static std::shared_ptr<Model> s_model;
	static std::shared_ptr<Model> s_modelHit;

private:

	/*===== メンバ変数 =====*/

	Vec3<float> m_pos;			// 座標
	Vec3<float> m_forwardVec;	// 移動方向ベクトル
	float m_scale;				// 大きさ
	float m_speed;				// 移動速度
	const float SPEED = 0.6f;	// 移動速度のデフォルト値。
	const float TRACKING_SPEED = 1.0f;
	bool m_isActive;			// 生存フラグ
	int m_hitEffectTimer;		// 弾に当たったフラグ
	const int HIT_EFFECT_TIMER = 10;

	int m_hp;
	int m_shotTimer;
	const int SHOT_TIMER = 3000;

	//モデル
	Transform m_transform;

	//enum class ID {

	//	STOPPING,	// 動かない敵。
	//	STRAIGHT,	// まっすぐ動く敵。
	//	PLAYER_STRAIGHT,	// 生成位置からプレイヤーの方向に真っ直ぐ動く敵。
	//	TRACKING,	// 追尾する敵。
	//	SHIELD,		// 盾持ちの敵。
	//	ID_COUNT,

	//};

	// スケール
	const float STOPPING_SCALE = 6.0f;
	const float STRAIGHT_SCALE = 3.0f;
	const float PLAYER_STRAIGHT_SCALE = 3.0f;
	const float TRACKING_SCALE = 3.0f;
	const float SHIELD_SCALE = 12.0f;

	// HP
	const int STOPPING_HP = 10;
	const int STRAIGHT_HP = 7;
	const int PLAYER_STRAIGHT_HP = 7;
	const int TRACKING_HP = 4;
	const int SHIELD_HP = 30;

	// 縁
	const float EDGE = 10.0f;

	const float TRACKING_ROT = 0.05f;
	const float SHIELD_ROT = 0.01f;

	ENEMY_INFO::ID m_id;


public:

	static bool m_debugIsShotEnemy;


public:

	/*===== メンバ関数 =====*/

	Enemy();
	void Init();
	void Generate(ENEMY_INFO::ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos, const Vec3<float> ForwardVec);
	void Update(std::weak_ptr< BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize);
	void Draw(Camera& Cam);

	bool GetIsActive() { return m_isActive; }
	Vec3<float> GetPos() { return m_pos; }
	float GetScale() { return m_scale; }

	// 近くにいるか。
	bool CheckIsEdge(const Vec3<float>& Pos, const float& Size);

	// 当たっているか。
	bool CheckHit(const Vec3<float>& Pos, const float& Size);

	// ダメージを与える。
	void Damage();

private:

	void CheckHitBullet(std::weak_ptr< BulletMgr> BulletMgr, const float& MapSize);

	void Shot(std::weak_ptr< BulletMgr> BulletMgr, const Vec3<float>& PlayerPos);

};