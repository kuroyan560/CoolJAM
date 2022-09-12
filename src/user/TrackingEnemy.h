#pragma once
#include "BaseEnemy.h"
#include "Transform.h"

class EnemyHP;

class TrackingEnemy : public BaseEnemy {

private:

	/*===== �����o�ϐ� =====*/


	Vec3<float> m_forwardVec;	// �ړ������x�N�g��
	Vec3<float> m_knockBackVec;
	float m_knockBackSpeed;
	const float KNOCK_BACK_SPEED = 2.5f;
	float m_speed;				// �ړ����x
	const float SPEED = 1.0f;	// �ړ����x�̃f�t�H���g�l�B
	int m_hitEffectTimer;		// �e�ɓ��������t���O
	const int HIT_EFFECT_TIMER = 10;

	int m_shotTimer;
	const int SHOT_TIMER = 300;

	//���f��
	Transform m_transform;

	// �X�P�[��
	const float SCALE = 3.0f;

	const float TRACKING_ROT = 0.02f;

	// HP
	static const int HP = 4;

	std::array<std::shared_ptr<EnemyHP>, HP> m_hpUI;


public:

	/*===== �����o�֐� =====*/

	TrackingEnemy(std::shared_ptr<Model> DefModel, std::shared_ptr<Model> DamageModel);
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
