#pragma once
#include"Transform.h"
#include"Vec.h"
#include<memory>
#include<array>

class Model;
class Camera;

class DriftParticle {

private:

	/*===== �����o�ϐ� =====*/

	// �g�����X�t�H�[��
	Transform m_transform;

	// ���f��
	static std::shared_ptr<Model>m_model;

	// ��{�I�ȕϐ�
	Vec3<float> m_pos;			// ���W
	Vec3<float> m_playerPos;
	Vec3<float> m_prevPos;		// �O�t���[���̍��W
	Vec3<float> m_nowFrameMoveVec;
	Vec3<float> m_prevFrameMoveVec;
	Vec3<float> m_forwardVec;	// �ړ������x�N�g��
	float m_speed;				// �ړ����x
	const float MAX_SPEED = 2.0f;
	const float SUB_SPEED = 0.5f;
	float m_scale;
	const float SCALE = 0.5f;	// �傫��
	const float SUB_SCALE = 0.25f;
	int m_exitTimer;			// ������܂ł̃^�C�}�[
	const int EXIT_TIMER = 0;
	float m_gravity;			// �d��
	const float ADD_GRAVITY = 0.01f;
	const float MAX_GRAVITY = 0.1f;
	bool m_isActive;			// �����t���O

public:

	DriftParticle();
	void Init();
	void Generate(const Vec3<float>& Pos, const float& Angle, const float& Rate);
	void Update(const Vec3<float>& Pos);
	void Draw(Camera& Cam);

	bool GetIsActive() { return m_isActive; }

};