#pragma once
#include "BaseEnemy.h"
#include "Transform.h"

class EnemyHP;

class StoppingEnemy : public BaseEnemy {

private:

	/*===== �����o�ϐ� =====*/

	Vec3<float> m_forwardVec;	// �ړ������x�N�g��
	Vec3<float> m_knockBackVec;
	float m_knockBackSpeed;
	const float KNOCK_BACK_SPEED = 1.0f;
	float m_speed;				// �ړ����x
	const float SPEED = 0.6f;	// �ړ����x�̃f�t�H���g�l�B
	int m_hitEffectTimer;		// �e�ɓ��������t���O
	const int HIT_EFFECT_TIMER = 10;

	// �X�P�[��
	const float SCALE = 6.0f;

	// HP
	static const int HP = 10;

	std::array<std::shared_ptr<EnemyHP>, HP> m_hpUI;



public:

	/*===== �����o�֐� =====*/

	StoppingEnemy(std::shared_ptr<Model> DefModel, std::shared_ptr<Model> DamageModel);
	void OnInit()override;
	void OnGenerate(ENEMY_INFO::ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos, const Vec3<float> ForwardVec)override;
	void OnUpdate(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize)override;
	void OnDraw()override;

private:

	// �e�Ƃ̓����蔻��B
	void CheckHitBullet(std::weak_ptr< BulletMgr> BulletMgr, const float& MapSize, const Vec3<float>& PlayerPos);

	// �e���������B
	void Shot(std::weak_ptr< BulletMgr> BulletMgr, const Vec3<float>& PlayerPos);

};
