#pragma once
#include "BaseEnemy.h"
#include "Transform.h"

class PressEnemy : public BaseEnemy {

private:

	/*===== �����o�ϐ� =====*/

	// ���f���B
	std::shared_ptr<Model> m_model;
	std::shared_ptr<Model> m_modelHit;


	Vec3<float> m_defPos;
	Vec3<float> m_pos;			// ���W
	Vec3<float> m_forwardVec;	// �ړ������x�N�g��
	Vec3<float> m_knockBackVec;
	float m_knockBackSpeed;
	float m_scale;				// �傫��
	float m_speed;				// �ړ����x
	float m_returnDefPosSpeed;	// �����ʒu�ɖ߂�ۂ̃X�s�[�h
	const float SPEED = 0.6f;	// �ړ����x�̃f�t�H���g�l�B
	bool m_isActive;			// �����t���O
	int m_hitEffectTimer;		// �e�ɓ��������t���O
	const int HIT_EFFECT_TIMER = 10;
	const float PRESS_KNOCK_BACK_SPEED = 1.0f;

	int m_hp;
	int m_shotTimer;
	const int SHOT_TIMER = 300;

	const float RETURN_DEFPOS_SPEED = 0.5f;

	//���f��
	Transform m_transform;

	// �X�P�[��
	const float PRESS_SCALE = 32.0f;

	// HP
	const int PRESS_HP = 60;

	ENEMY_INFO::ID m_id;


public:

	/*===== �����o�֐� =====*/

	PressEnemy(std::shared_ptr<Model> DefModel, std::shared_ptr<Model> DamageModel);
	void Init()override;
	void Generate(ENEMY_INFO::ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos, const Vec3<float> ForwardVec)override;
	void Update(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize)override;
	void Draw()override;
	bool GetIsActive()override { return m_isActive; };
	float GetScale()override { return m_scale; };
	Vec3<float> GetPos()override { return m_pos; };

private:

	// �e�Ƃ̓����蔻��B
	void CheckHitBullet(std::weak_ptr< BulletMgr> BulletMgr, const float& MapSize, const Vec3<float>& PlayerPos);

	// �e���������B
	void Shot(std::weak_ptr< BulletMgr> BulletMgr, const Vec3<float>& PlayerPos);

};