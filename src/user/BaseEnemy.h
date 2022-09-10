#pragma once
#include "Vec.h"
#include "EnemyWave.h"
#include <memory>

class Model;
class Camera;
class BulletMgr;

class BaseEnemy {

public:

	/*===== �����o�֐� =====*/

	// ���f���B
	std::shared_ptr<Model> m_model;
	std::shared_ptr<Model> m_modelHit;

	Vec3<float> m_pos;	// ���W
	int m_hp;			// HP
	float m_scale;		// �傫��
	bool m_isActive;	// �����t���O
	ENEMY_INFO::ID m_id;


public:

	/*===== �����o�֐� =====*/

	virtual void Init() = 0;
	virtual void Generate(ENEMY_INFO::ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos, const Vec3<float> ForwardVec) = 0;
	virtual void Update(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize) = 0;
	virtual void Draw() = 0;

	void Damage(const int& Amount);

};