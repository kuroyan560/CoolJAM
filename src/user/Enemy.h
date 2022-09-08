#pragma once
#include "Vec.h"
#include "Transform.h"
#include <memory>
#include <array>

class Model;
class Camera;
class BulletMgr;

class Enemy {

private:

	/*===== メンバ変数 =====*/

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
	std::shared_ptr<Model> m_model;
	std::shared_ptr<Model> m_modelHit;
	Transform m_transform;

	const std::array<float, 3> SCALE = { 6,3,1 };
	const std::array<float, 3> EDGE_SCALE = { 10,7,4 };	// 近くにいる判定の大きさ。
	const std::array<int, 3> HP = { 10,5,3 };
	const float TRACKING_ROT = 0.05f;


public:

	enum class ID {

		STOPPING,
		STRAIGHT,
		TRACKING,

	};

	static bool m_debugIsShotEnemy;

private:

	ID m_id;


public:

	/*===== メンバ関数 =====*/

	Enemy();
	void Init();
	void Generate(ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos);
	void Update(std::weak_ptr< BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize);
	void Draw(Camera& Cam);

	bool GetIsActive() { return m_isActive; }
	Vec3<float> GetPos() { return m_pos; }
	float GetScale() { return SCALE[static_cast<int>(m_id)]; }

	// 近くにいるか。
	bool CheckIsEdge(const Vec3<float>& Pos, const float& Size);

	// ダメージを与える。
	void Damage();

private:

	void CheckHit(std::weak_ptr< BulletMgr> BulletMgr, const float& MapSize);

	void Shot(std::weak_ptr< BulletMgr> BulletMgr, const Vec3<float>& PlayerPos);

};