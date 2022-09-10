#pragma once
#include "BaseEnemy.h"
#include "Transform.h"

class StraightEnemy : public BaseEnemy {

private:

	/*===== �����o�ϐ� =====*/

	Vec3<float> m_forwardVec;	// �ړ������x�N�g��
	float m_speed;				// �ړ����x
	const float SPEED = 0.6f;	// �ړ����x�̃f�t�H���g�l�B
	int m_hitEffectTimer;		// �e�ɓ��������t���O
	const int HIT_EFFECT_TIMER = 10;

	int m_shotTimer;
	const int SHOT_TIMER = 300;

	//���f��
	Transform m_transform;

	// �X�P�[��
	const float STRAIGHT_SCALE = 3.0f;

	// HP
	const int STRAIGHT_HP = 7;


public:

	/*===== �����o�֐� =====*/

	StraightEnemy(std::shared_ptr<Model> DefModel, std::shared_ptr<Model> DamageModel);
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
