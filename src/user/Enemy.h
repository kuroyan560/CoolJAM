#pragma once
#include "Vec.h"
#include "Transform.h"
#include "EnemyWave.h"
#include <memory>
#include <array>

class Model;
class Camera;
class BulletMgr;
class UnionEnemy;

class Enemy {

	static std::shared_ptr<Model> s_model;
	static std::shared_ptr<Model> s_modelHit;

private:

	/*===== �����o�ϐ� =====*/

	Vec3<float> m_pos;			// ���W
	Vec3<float> m_forwardVec;	// �ړ������x�N�g��
	Vec3<float> m_knockBackVec;
	float m_knockBackSpeed;
	const float KNOCK_BACK_SPEED = 2.5f;
	float m_scale;				// �傫��
	float m_speed;				// �ړ����x
	const float SPEED = 0.6f;	// �ړ����x�̃f�t�H���g�l�B
	const float TRACKING_SPEED = 1.3f;
	bool m_isActive;			// �����t���O
	int m_hitEffectTimer;		// �e�ɓ��������t���O
	const int HIT_EFFECT_TIMER = 10;

	int m_hp;
	int m_shotTimer;
	const int SHOT_TIMER = 300;

	//���f��
	Transform m_transform;

	// UNION�̓G�̎���̓G
	std::array<std::shared_ptr<UnionEnemy>, 6> m_unionEnemy;

	//enum class ID {

	//	STOPPING,	// �����Ȃ��G�B
	//	STRAIGHT,	// �܂����������G�B
	//	PLAYER_STRAIGHT,	// �����ʒu����v���C���[�̕����ɐ^�����������G�B
	//	TRACKING,	// �ǔ�����G�B
	//	SHIELD,		// �������̓G�B
	//	UNION,		// �W���̂̓G�B
	//	ID_COUNT,

	//};

	// �X�P�[��
	const float STOPPING_SCALE = 6.0f;
	const float STRAIGHT_SCALE = 3.0f;
	const float PLAYER_STRAIGHT_SCALE = 3.0f;
	const float TRACKING_SCALE = 3.0f;
	const float SHIELD_SCALE = 12.0f;
	const float UNION_SCALE = 6.0f;

	// HP
	const int STOPPING_HP = 10;
	const int STRAIGHT_HP = 7;
	const int PLAYER_STRAIGHT_HP = 7;
	const int TRACKING_HP = 4;
	const int SHIELD_HP = 30;
	const int UNION_HP = 5;

	// ��
	const float EDGE = 10.0f;

	const float TRACKING_ROT = 0.03f;
	const float SHIELD_ROT = 0.01f;

	ENEMY_INFO::ID m_id;


public:

	static bool m_debugIsShotEnemy;


public:

	/*===== �����o�֐� =====*/

	Enemy();
	void Init();
	void Generate(ENEMY_INFO::ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos, const Vec3<float> ForwardVec);
	void Update(std::weak_ptr< BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize);
	void Draw(Camera& Cam);

	bool GetIsActive() { return m_isActive; }
	Vec3<float> GetPos() { return m_pos; }
	float GetScale() { return m_scale; }

	// �}�b�v���Ɏ��܂�悤�ɂ���B
	Vec3<float> KeepInMap(Vec3<float>& Pos, const float& MapSize);

	// �߂��ɂ��邩�B
	bool CheckIsEdge(const Vec3<float>& Pos, const float& Size);

	// �������Ă��邩�B
	bool CheckHit(const Vec3<float>& Pos, const float& Size);

	// �_���[�W��^����B
	void Damage();

private:

	void CheckHitBullet(std::weak_ptr< BulletMgr> BulletMgr, const float& MapSize, const Vec3<float>& PlayerPos);

	void Shot(std::weak_ptr< BulletMgr> BulletMgr, const Vec3<float>& PlayerPos);

};