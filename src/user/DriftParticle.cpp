#include "DriftParticle.h"
#include "Importer.h"
#include "Model.h"

DriftParticle::DriftParticle() {

	/*===== �R���X�g���N�^ =====*/

	// ���f��
	m_model = Importer::Instance()->LoadModel("resource/user/", "playerDriftParticle.glb");;

	// ��{�I�ȕϐ�
	m_pos = Vec3<float>();			// ���W
	m_prevPos = Vec3<float>();		// �O�t���[���̍��W
	m_forwardVec = Vec3<float>();	// �ړ������x�N�g��
	m_nowFrameMoveVec = Vec3<float>();	// �O�t���[���̍��W
	m_prevFrameMoveVec = Vec3<float>();	// �ړ������x�N�g��
	m_speed = 0;					// �ړ����x
	m_exitTimer = 0;				// ������܂ł̃^�C�}�[
	m_scale = SCALE;				// �傫��
	m_gravity = 0;					// �d��
	m_isActive = false;				// �����t���O

}

void DriftParticle::Init() {

	/*===== ���������� =====*/

	// ��{�I�ȕϐ�
	m_pos = Vec3<float>();			// ���W
	m_prevPos = Vec3<float>();		// �O�t���[���̍��W
	m_forwardVec = Vec3<float>();	// �ړ������x�N�g��
	m_nowFrameMoveVec = Vec3<float>();	// �O�t���[���̍��W
	m_prevFrameMoveVec = Vec3<float>();	// �ړ������x�N�g��
	m_speed = 0;					// �ړ����x
	m_exitTimer = 0;				// ������܂ł̃^�C�}�[
	m_scale = SCALE;				// �傫��
	m_gravity = 0;					// �d��
	m_isActive = false;				// �����t���O

}

void DriftParticle::Generate(const Vec3<float>& Pos, const float& Angle, const float& Rate) {

	/*===== �������� =====*/

	m_playerPos = Pos;
	m_pos = Pos;
	m_prevPos = Pos;
	m_speed = KuroFunc::GetRand(MAX_SPEED / 2.0f, MAX_SPEED);
	m_scale = SCALE;
	m_gravity = 0;
	m_exitTimer = 0;
	m_isActive = true;
	m_nowFrameMoveVec = Vec3<float>();	// �O�t���[���̍��W
	m_prevFrameMoveVec = Vec3<float>();	// �ړ������x�N�g��

	m_transform.SetRotate(DirectX::XMMatrixRotationY(Angle + DirectX::XM_PIDIV2));	// �p�x���߁B

	// �ړ������x�N�g�������߂�B
	float randomAmount = 0.9f;
	m_forwardVec = Vec3<float>(sinf(Angle + KuroFunc::GetRand(-randomAmount, randomAmount)), 0.0f, cosf(Angle + KuroFunc::GetRand(-0.5f, randomAmount)));
	//m_forwardVec.y = KuroFunc::GetRand(-500, 500) / 1000.0f;
	//m_forwardVec.y += (Rate * 0.5f);
	m_forwardVec.y += 0.1f;
	m_forwardVec.Normalize();

}

void DriftParticle::Update(const Vec3<float>& Pos) {

	/*===== �X�V���� =====*/

	// ���W��ۑ��B
	m_prevPos = m_pos;

	// �d�͂��v�Z�B
	m_gravity += ADD_GRAVITY;
	if (MAX_GRAVITY < m_gravity) m_gravity = MAX_GRAVITY;

	// �ړ�������B
	m_pos += m_forwardVec * m_speed;

	// �ړ����x�����炷�B
	m_speed -= SUB_SPEED;
	if (m_speed < SUB_SPEED) {
		m_speed = SUB_SPEED;
	}

	// �d�͂����Z�B
	//m_pos -= m_gravity;

	// ������܂ł̃^�C�}�[���X�V�B
	++m_exitTimer;
	if (EXIT_TIMER < m_exitTimer) {

		// �X�P�[��������������B
		m_scale -= 0.05f;
		if (m_scale < 0) {

			Init();

		}

	}

	// �O�t���[���̈ړ��x�N�g����ۑ��B
	m_prevFrameMoveVec = m_nowFrameMoveVec;
	m_nowFrameMoveVec = Vec3<float>(m_pos - m_prevPos).GetNormal();

}

#include "DrawFunc_Append.h"
void DriftParticle::Draw() {

	/*===== �`�揈�� =====*/

	m_transform.SetPos(m_pos);
	m_transform.SetScale(m_scale);
	DrawFunc_Append::DrawModel(m_model, m_transform, 1.0f, { false,true,false });

}