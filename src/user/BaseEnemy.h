#pragma once
#include "Vec.h"
#include "EnemyWave.h"
#include <memory>

class Model;
class Camera;
class BulletMgr;

class BaseEnemy {

public:

	/*===== メンバ関数 =====*/

	// モデル。
	std::shared_ptr<Model> m_model;
	std::shared_ptr<Model> m_modelHit;

	Vec3<float> m_pos;	// 座標
	int m_hp;			// HP
	float m_scale;		// 大きさ
	bool m_isActive;	// 生存フラグ
	ENEMY_INFO::ID m_id;


public:

	/*===== メンバ関数 =====*/

	virtual void Init() = 0;
	virtual void Generate(ENEMY_INFO::ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos, const Vec3<float> ForwardVec) = 0;
	virtual void Update(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize) = 0;
	virtual void Draw() = 0;

	void Damage(const int& Amount, std::weak_ptr<BulletMgr> BulletMgr);

};