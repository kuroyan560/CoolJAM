#pragma once
#include "BaseEnemy.h"
#include "Transform.h"

class PressEnemy : public BaseEnemy {

private:

	/*===== �����o�ϐ� =====*/



	Vec3<float> m_defPos;
	Vec3<float> m_forwardVec;	// �ړ������x�N�g��
	Vec3<float> m_knockBackVec;
	float m_knockBackSpeed;
	float m_speed;				// �ړ����x
	float m_returnDefPosSpeed;	// �����ʒu�ɖ߂�ۂ̃X�s�[�h
	const float SPEED = 0.6f;	// �ړ����x�̃f�t�H���g�l�B
	int m_hitEffectTimer;		// �e�ɓ��������t���O
	const int HIT_EFFECT_TIMER = 10;
	const float MAX_KNOCK_BACK_SPEED = 3.5f;
	const float ADD_KNOCK_BACK_SPEED = 0.2f;
	int m_returnDefTimer;
	const int RETURN_DEF_TIMER = 120;

	int m_shotTimer;
	const int SHOT_TIMER = 300;

	const float RETURN_DEFPOS_SPEED = 0.2f;

	//���f��
	Transform m_transform;

	// �X�P�[��
	const float PRESS_SCALE = 32.0f;

	// HP
	const int PRESS_HP = 60;


public:

	/*===== �����o�֐� =====*/

	PressEnemy(std::shared_ptr<Model> DefModel, std::shared_ptr<Model> DamageModel);
	void Init()override;
	void Generate(ENEMY_INFO::ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos, const Vec3<float> ForwardVec)override;
	void Update(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize)override;
	void Draw(Camera& Cam)override;

private:

	// �e�Ƃ̓����蔻��B
	void CheckHitBullet(std::weak_ptr< BulletMgr> BulletMgr, const float& MapSize, const Vec3<float>& PlayerPos);

	// �e���������B
	void Shot(std::weak_ptr< BulletMgr> BulletMgr, const Vec3<float>& PlayerPos);

};