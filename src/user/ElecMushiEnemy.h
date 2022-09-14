#pragma once
#include "BaseEnemy.h"
#include "Transform.h"

class EnemyHP;

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


	// �X�P�[��
	const float SCALE = 3.0f;

	// HP
	static const int HP = 15;

	std::array<std::shared_ptr<EnemyHP>, HP> m_hpUI;


public:

	/*===== �����o�֐� =====*/

	ElecMushiEnemy(std::shared_ptr<Model> DefModel, std::shared_ptr<Model> DamageModel);
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