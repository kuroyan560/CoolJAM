#pragma once
#include"Transform.h"
#include "Vec.h"
#include<memory>
#include<DirectXMath.h>

class Model;
class Camera;

class Player
{

private:

	/*===== �����o�ϐ� =====*/

	//�g�����X�t�H�[��
	Transform m_transform;
	Vec3<float> m_pos;			// ���ݍ��W
	Vec3<float> m_inputVec;		// ���͂��ꂽ�����x�N�g��(�ړ������x�N�g�������̕����ɕ⊮����B)
	Vec3<float> m_forwardVec;	// �ړ������x�N�g��
	const Vec3<float> DEF_FORWARDVEC = Vec3<float>(0.0f, 0.0f, 1.0f);
	float m_speed;				// �ړ����x
	const float DEF_SPEED = 1.0f;			// �f�t�H���g�̈ړ����x
	const float BRAKE_SPEED = 0.6f;			// �u���[�L���̈ړ����x
	const float HANDLE_ROT = 0.03f;			// �ړ������x�N�g���̕ۊǗʃ��W�A��
	const float BRAKE_HANDLE_ROT = 0.06f;	// �u���[�L���̈ړ������x�N�g���̕ۊǗʃ��W�A��
	bool m_isEdge;				// ���ɂ��邩

	// �u���[�L�֌W
	int m_brakeTimer;			// �u���[�L���Ă��鎞�ԁB
	float m_brakeBoostSpeed;	// �u���[�L�ɂ���Ĕ��������u�[�X�g�̑��x�B
	const int MAX_BRAKE_TIMER = 60.0f;
	const float MAX_BRAKE_SPEED = 5.0f;
	const float SUB_BRAKE_SPEED = 0.1f;
	bool m_isBrake;				// �u���[�L���Ă��邩�ǂ����B
	//���f��
	std::shared_ptr<Model>m_model;

	float inputATan2f;

public:

	/*===== �����o�֐� =====*/

	Player();
	void Init();
	void Finalize();
	void Update(const float& MapSize, const float& EdgeScope);
	void Draw(Camera& Cam);

	Vec3<float> GetPos() { return m_pos; };
	Vec3<float>* GetPosPtr() { return &m_pos; };
	const float *GetInputRadianPtr() { return &inputATan2f; };
	float GetInputRadian() { return inputATan2f; };

private:

	// ���͏���
	void Input();

	// �ړ�����
	void Move();

	// �����蔻��
	void CheckHit(const float& MapSize, const float& EdgeScope);

	// 01�ɔ[�߂�B
	float Saturate(const float& Value);

	// �g�����X�t�H�[���Q�b�^
	const Transform& GetTransform()const { return m_transform; }

};

