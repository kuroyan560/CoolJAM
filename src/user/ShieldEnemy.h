#pragma once
#include "BaseEnemy.h"
#include "Transform.h"

class EnemyHP;

class ShieldEnemy : public BaseEnemy {

private:

	/*===== �����o�ϐ� =====*/

	// ���f���B
	std::shared_ptr<Model> m_model;
	std::shared_ptr<Model> m_modelHit;


	Vec3<float> m_pos;			// ���W
	Vec3<float> m_forwardVec;	// �ړ������x�N�g��
	float m_scale;				// �傫��
	float m_speed;				// �ړ����x
	const float SPEED = 0.6f;	// �ړ����x�̃f�t�H���g�l�B
	bool m_isActive;			// �����t���O
	int m_hitEffectTimer;		// �e�ɓ��������t���O
	const int HIT_EFFECT_TIMER = 10;

	int m_hp;
	int m_shotTimer;
	const int SHOT_TIMER = 300;

	//���f��
	Transform m_transform;

	// �X�P�[��
	const float SCALE = 12.0f;

	// HP
	static const int HP = 30;

	const float SHIELD_ROT = 0.01f;

	ENEMY_INFO::ID m_id;

	std::array<std::shared_ptr<EnemyHP>, HP> m_hpUI;


public:

	/*===== �����o�֐� =====*/

	ShieldEnemy(std::shared_ptr<Model> DefModel, std::shared_ptr<Model> DamageModel);
	void Init()override;
	void Generate(ENEMY_INFO::ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos, const Vec3<float> ForwardVec)override;
	void Update(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize)override;
	void Draw()override;

private:

	// �e�Ƃ̓����蔻��B
	void CheckHitBullet(std::weak_ptr< BulletMgr> BulletMgr, const float& MapSize, const Vec3<float>& PlayerPos);

	// �e���������B
	void Shot(std::weak_ptr< BulletMgr> BulletMgr, const Vec3<float>& PlayerPos);

};
