#pragma once
#include "BaseEnemy.h"
#include "Transform.h"
#include <array>
#include <memory>

class UnionEnemy;

class UnionBaseEnemy : public BaseEnemy {

private:

	/*===== �����o�ϐ� =====*/


	Vec3<float> m_forwardVec;	// �ړ������x�N�g��
	Vec3<float> m_knockBackVec;
	float m_knockBackSpeed;
	const float KNOCK_BACK_SPEED = 2.5f;
	float m_speed;				// �ړ����x
	const float SPEED = 0.6f;	// �ړ����x�̃f�t�H���g�l�B
	int m_hitEffectTimer;		// �e�ɓ��������t���O
	const int HIT_EFFECT_TIMER = 10;

	int m_shotTimer;
	const int SHOT_TIMER = 300;

	// UNION�̓G�̎���̓G
	std::array<std::shared_ptr<UnionEnemy>, 6> m_unionEnemy;

	//���f��
	Transform m_transform;

	// �X�P�[��
	const float UNION_SCALE = 6.0f;

	// HP
	const int UNION_HP = 5;


public:

	/*===== �����o�֐� =====*/

	UnionBaseEnemy(std::shared_ptr<Model> DefModel, std::shared_ptr<Model> DamageModel);
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