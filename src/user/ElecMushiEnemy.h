#pragma once
#include "BaseEnemy.h"
#include "Transform.h"

class ElecMushiEnemy : public BaseEnemy {

private:

	/*===== �����o�ϐ� =====*/


	Vec3<float> m_forwardVec;	// �ړ������x�N�g��
	float m_speed;				// �ړ����x
	const float SPEED = 0.6f;	// �ړ����x�̃f�t�H���g�l�B
	int m_hitEffectTimer;		// �e�ɓ��������t���O
	const int HIT_EFFECT_TIMER = 10;

	float m_sineWaveTimer;
	const float ADD_SINE_WAVE_TIMER = 0.1f;
	const float SINE_WAVE_LENGTH = 10.0f;
	float m_sineWaveLength;
	float m_defLength;

	int m_shotTimer;
	const int SHOT_TIMER = 300;

	// ���f��
	Transform m_transform;

	// �X�P�[��
	const float PLAYER_STRAIGHT_SCALE = 3.0f;

	// HP
	const int PLAYER_STRAIGHT_HP = 7;


public:

	/*===== �����o�֐� =====*/

	ElecMushiEnemy(std::shared_ptr<Model> DefModel, std::shared_ptr<Model> DamageModel);
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