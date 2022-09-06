#include "Player.h"
#include "../engine/UsersInput.h"
#include"Importer.h"
#include "Model.h"
#include"KazCollisionHelper.h"

Player::Player()
{

	/*===== �R���X�g���N�^ =====*/

	m_pos = Vec3<float>();
	m_inputVec = Vec3<float>();
	m_forwardVec = DEF_FORWARDVEC;
	m_speed = 0;
	m_brakeTimer = 0;
	m_brakeBoostSpeed = 0;
	m_isEdge = false;
	m_isBrake = false;

	m_model = Importer::Instance()->LoadModel("resource/user/", "player.glb");

}

void Player::Init()
{

	/*===== ���������� =====*/

	m_pos = Vec3<float>();
	m_inputVec = Vec3<float>();
	m_forwardVec = DEF_FORWARDVEC;
	m_speed = 0;
	m_brakeTimer = 0;
	m_brakeBoostSpeed = 0;
	m_isEdge = false;
	m_isBrake = false;

}

void Player::Update(const float& MapSize, const float& EdgeScope)
{

	/*===== �X�V���� =====*/

	// ���͏���
	Input();

	// �ړ�����
	Move();

	// �����蔻�菈��
	CheckHit(MapSize, EdgeScope);

}

void Player::Input()
{

	/*====== ���͏��� =====*/

	// �X�e�B�b�N�̓��͂��擾�B
	Vec2<float> stickInput = UsersInput::Instance()->GetLeftStickVecRawFuna(0);

	// �f�b�h���C����݂���B
	const float INPUT_DEADLINE = 0.25f;
	// ���͂���Ă�����B
	if (INPUT_DEADLINE < stickInput.Length()) {

		// ���͂�ۑ��B
		m_inputVec = Vec3<float>(stickInput.y, 0.0f, stickInput.x);

	}

	// �u���[�L���͂�ۑ��B
	bool isBrakeBoostNow = 0.1f < m_brakeBoostSpeed;
	m_isBrake = UsersInput::Instance()->ControllerInput(0, A) && !isBrakeBoostNow;
	if (m_isBrake) {

		++m_brakeTimer;

	}
	else {

		// �u���[�L�^�C�}�[��1�ȏゾ������u���[�L�𗣂����u�ԂƂ������ƁB
		if (0 < m_brakeTimer) {

			// �o�ߎ��Ԃ��犄�������߂�B
			float brakeRate = Saturate(static_cast<float>(m_brakeTimer) / static_cast<float>(MAX_BRAKE_TIMER));

			// �u�[�X�g�ʂ��X�V�B
			float boostAmount = MAX_BRAKE_SPEED * brakeRate;
			if (m_brakeBoostSpeed < boostAmount) {

				m_brakeBoostSpeed = boostAmount;

			}

		}

		m_brakeTimer = 0;

	}

	// �f�o�b�O�@�\
	if (UsersInput::Instance()->ControllerInput(0, B)) {

		Init();

	}

}

void Player::Move()
{

	/*===== �ړ����� =====*/

	// �ړ������x�N�g������͕����x�N�g���Ɍ������ĕ⊮����B
	float cross = m_forwardVec.Cross(m_inputVec).y;
	if (cross != 0) {

		cross = cross < 0 ? -1.0f : 1.0f;
		float handleRot = (m_isBrake ? BRAKE_HANDLE_ROT : HANDLE_ROT) * cross;

		// �ړ������x�N�g�����p�x�ɒ����Ēl�����Z����B
		float forwardVecAngle = atan2f(m_forwardVec.x, m_forwardVec.z);
		forwardVecAngle += handleRot;

		// ���Z�����p�x���x�N�g���ɒ����B
		m_forwardVec = Vec3<float>(sinf(forwardVecAngle), 0.0f, cosf(forwardVecAngle));

	}

	// �u���[�L��Ԃ̗L���ɉ����Ĉړ����x��ς���B
	const float BASE_SPEED = m_isBrake ? BRAKE_SPEED : DEF_SPEED;

	// �ړ����x��⊮����B
	m_speed += (BASE_SPEED - m_speed) / 20.0f;

	// �u�[�X�g������������B
	m_brakeBoostSpeed -= SUB_BRAKE_SPEED;
	if (m_brakeBoostSpeed < 0.0f) {

		m_brakeBoostSpeed = 0.0f;

	}

	// �ړ�������B
	m_pos += m_forwardVec * m_speed;

	// �u�[�X�g���ړ�������B
	m_pos += m_forwardVec * m_brakeBoostSpeed;

}

#include"DrawFunc3D.h"
void Player::Draw(Camera& Cam) {

	/*===== �`�揈�� =====*/

	m_transform.SetPos(m_pos);

	// ���͂̊p�x�����߂�B
	inputATan2f = atan2f(m_inputVec.x, m_inputVec.z);
	m_transform.SetRotate(DirectX::XMMatrixRotationY(inputATan2f));

	DrawFunc3D::DrawNonShadingModel(m_model, m_transform, Cam);

}

void Player::CheckHit(const float& MapSize, const float& EdgeScope)
{

	/*===== �����蔻�� =====*/

	// �}�b�v�Ƃ̓����蔻��B
	m_pos = KazCollisionHelper::KeepInMap(m_pos, MapSize);

	// �G�b�W�̔���B
	m_isEdge = MapSize - m_pos.Length() < EdgeScope;

}

void Player::Finalize()
{


}


float Player::Saturate(const float& Value)
{

	/*===== 01�ɔ[�߂�B =====*/

	float value = Value;
	if (value < 0.0f) value = 0.0f;
	if (1.0f < value) value = 1.0f;

	return value;
}
