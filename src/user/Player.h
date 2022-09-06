#pragma once
#include "Vec.h"

class Player {

private:

	/*===== �����o�ϐ� =====*/

	Vec3<float> m_pos;			// ���ݍ��W
	Vec3<float> m_inputVec;		// ���͂��ꂽ�����x�N�g��(�ړ������x�N�g�������̕����ɕ⊮����B)
	Vec3<float> m_forwardVec;	// �ړ������x�N�g��
	const Vec3<float> DEF_FORWARDVEC = Vec3<float>(0.0f, 0.0f, 1.0f);
	float m_speed;				// �ړ����x
	const float DEF_SPEED = 10.0f;			// �f�t�H���g�̈ړ����x
	const float BRAKE_SPEED = 3.0f;			// �u���[�L���̈ړ����x
	const float HANDLE_ROT = 0.03f;			// �ړ������x�N�g���̕ۊǗʃ��W�A��
	const float BRAKE_HANDLE_ROT = 0.06f;	// �u���[�L���̈ړ������x�N�g���̕ۊǗʃ��W�A��
	bool m_isEdge;				// ���ɂ��邩
	const float EDGE_SIZE = 100.0f;

	// �u���[�L�֌W
	int m_brakeTimer;			// �u���[�L���Ă��鎞�ԁB
	float m_brakeBoostSpeed;	// �u���[�L�ɂ���Ĕ��������u�[�X�g�̑��x�B
	const int MAX_BRAKE_TIMER = 120.0f;
	const float MAX_BRAKE_SPEED = 60.0f;
	const float SUB_BRAKE_SPEED = 1.0f;
	bool m_isBrake;				// �u���[�L���Ă��邩�ǂ����B


public:

	/*===== �����o�֐� =====*/

	Player();
	void Init();
	void Update();
	void Draw();

	Vec3<float> GetPos() { return m_pos; }


private:

	// ���͏���
	void Input();

	// �ړ�����
	void Move();

	// �����蔻��
	void CheckHit();

	// 01�ɔ[�߂�B
	float Saturate(const float& Value);

};