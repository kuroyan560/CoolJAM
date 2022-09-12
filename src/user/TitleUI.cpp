#include "TitleUI.h"
#include "D3D12App.h"

TitleUI::TitleUI(std::string TexPath, STATUS Status, UI_STATUS UIStatus) {

	/*===== �R���X�g���N�^ =====*/

	m_texBuffer = D3D12App::Instance()->GenerateTextureBuffer(TexPath);
	m_status = Status;
	m_nextStatus = Status;
	m_defStatus = Status;
	m_UIStatus = UIStatus;
	m_easingTimer = 1.0f;

	switch (Status)
	{
	case TitleUI::STATUS::UP:

		m_angle = DEF_ROTATE[0];
		m_alpha = SIDE_ALPHA;

		break;
	case TitleUI::STATUS::MIDDLE:

		m_angle = DEF_ROTATE[1];
		m_alpha = 1.0f;

		break;
	case TitleUI::STATUS::DOWN:

		m_angle = DEF_ROTATE[2];
		m_alpha = SIDE_ALPHA;

		break;
	default:
		break;
	}

}

void TitleUI::Init() {

	/*===== ���������� =====*/

	m_status = m_defStatus;
	m_nextStatus = m_defStatus;
	m_easingTimer = 1.0f;

	switch (m_defStatus)
	{
	case TitleUI::STATUS::UP:

		m_angle = DEF_ROTATE[0];
		m_alpha = SIDE_ALPHA;

		break;
	case TitleUI::STATUS::MIDDLE:

		m_angle = DEF_ROTATE[1];
		m_alpha = 1.0f;

		break;
	case TitleUI::STATUS::DOWN:

		m_angle = DEF_ROTATE[2];
		m_alpha = SIDE_ALPHA;

		break;
	default:
		break;
	}

}

void TitleUI::Update() {

	/*===== �X�V���� =====*/

	// ���̃X�e�[�^�X�Ǝ��̃X�e�[�^�X���Ⴉ������B
	if (m_status != m_nextStatus) {

		// ���ݏゾ������B
		if (m_status == STATUS::UP) {

			ChangeUpdateNowUp();

		}
		// ���ݐ^�񒆂�������B
		else if (m_status == STATUS::MIDDLE) {

			ChangeUpdateNowMiddle();

		}
		// ���݉���������B
		else if (m_status == STATUS::DOWN) {

			ChangeUpdateNowDown();

		}

	}

	// �C�[�W���O���I�������B
	if (1.0f <= m_easingTimer) {

		m_status = m_nextStatus;

	}

}

#include "DrawFunc2D.h"
void TitleUI::Draw(const Vec2<float>& RevolverPos) {

	/*===== �`�揈�� =====*/

	// �x�N�g�������߂�B
	Vec2<float> vec = Vec2<float>(cosf(m_angle), sinf(m_angle));

	// UI�̎�ނɂ����X���̂��炵�ʂ����߂�B
	Vec2<float> zureX;
	switch (m_UIStatus)
	{
	case TitleUI::UI_STATUS::TUTORIAL:
		zureX.x = 200.0f;
		break;
	case TitleUI::UI_STATUS::GAME:
		zureX.x = 250.0f;
		break;
	case TitleUI::UI_STATUS::EXIT:
		zureX.x = 45.0f;
		break;
	default:
		break;
	}

	Vec2<float> centerPos = RevolverPos + zureX + vec * (100.0f + 200.0f);

	DrawFunc2D::DrawRotaGraph2D(centerPos, Vec2<float>(1.0f, 1.0f), 0, m_texBuffer, m_alpha);

}

void TitleUI::ChangeUpdateNowUp() {

	/*===== ���݂�STATUS���ゾ������ =====*/

	// �^�C�}�[���X�V�B
	m_easingTimer += ADD_EASING_TIMER;
	if (1.0f <= m_easingTimer) m_easingTimer = 1.0f;

	// ���̃X�e�[�^�X������������B
	if (m_nextStatus == STATUS::DOWN) {

		// �^�C�}�[�������ȉ���������A�A���t�@�l�������Ĉړ�������B
		if (m_easingTimer < 0.5f) {

			m_alpha = SIDE_ALPHA - ((m_easingTimer * 2.0f) * SIDE_ALPHA);
			m_angle = DEF_ROTATE[0] - ((m_easingTimer * 2.0f) * ROTATE_REVOLVER_UI);

		}
		else {

			m_alpha = (m_easingTimer * 1.0f) * SIDE_ALPHA;
			m_angle = DEF_ROTATE[2] + (ROTATE_REVOLVER_UI - (m_easingTimer)*ROTATE_REVOLVER_UI);

		}

	}
	// ���̃X�e�[�^�X���^�񒆂�������B
	else if (m_nextStatus == STATUS::MIDDLE) {

		// �ړ�������B
		m_alpha = SIDE_ALPHA + (m_easingTimer * (1.0f - SIDE_ALPHA));
		m_angle = DEF_ROTATE[0] + ROTATE_REVOLVER_UI * m_easingTimer;

	}

}

void TitleUI::ChangeUpdateNowMiddle() {

	/*===== ���݂�STATUS���^�񒆂������� =====*/

	// �^�C�}�[���X�V�B
	m_easingTimer += ADD_EASING_TIMER;
	if (1.0f <= m_easingTimer) m_easingTimer = 1.0f;

	// ���̃X�e�[�^�X���ゾ������B
	if (m_nextStatus == STATUS::UP) {

		m_alpha = 1.0f - (m_easingTimer * (1.0f - SIDE_ALPHA));
		m_angle = DirectX::XM_2PI - (m_easingTimer * ROTATE_REVOLVER_UI);

	}
	// ���̃X�e�[�^�X������������B
	else if (m_nextStatus == STATUS::DOWN) {

		m_alpha = 1.0f - (m_easingTimer * (1.0f - SIDE_ALPHA));
		m_angle = m_easingTimer * ROTATE_REVOLVER_UI;

	}

}

void TitleUI::ChangeUpdateNowDown() {

	/*===== ���݂�STATUS������������ =====*/

	// �^�C�}�[���X�V�B
	m_easingTimer += ADD_EASING_TIMER;
	if (1.0f <= m_easingTimer) m_easingTimer = 1.0f;

	// ���̃X�e�[�^�X���ゾ������B
	if (m_nextStatus == STATUS::UP) {

		// �^�C�}�[�������ȉ���������A�A���t�@�l�������Ĉړ�������B
		if (m_easingTimer < 0.5f) {

			m_alpha = SIDE_ALPHA - ((m_easingTimer * 2.0f) * SIDE_ALPHA);
			m_angle = DEF_ROTATE[2] + ((m_easingTimer * 2.0f) * ROTATE_REVOLVER_UI);

		}
		else {

			m_alpha = (m_easingTimer * 1.0f) * SIDE_ALPHA;
			m_angle = DEF_ROTATE[0] + (-ROTATE_REVOLVER_UI + (m_easingTimer)*ROTATE_REVOLVER_UI);

		}

	}
	// ���̃X�e�[�^�X���^�񒆂�������B
	else if (m_nextStatus == STATUS::MIDDLE) {

		// �ړ�������B
		m_alpha = SIDE_ALPHA + (m_easingTimer * (1.0f - SIDE_ALPHA));
		m_angle = DEF_ROTATE[2] - ROTATE_REVOLVER_UI * m_easingTimer;

	}

}

void TitleUI::Trans(STATUS Status) {

	/*===== ���̃X�e�[�^�X�����߂� =====*/

	// ���̃X�e�[�^�X���^�񒆂�������B
	if (m_status == STATUS::MIDDLE) {

		// ���̃X�e�[�^�X���ゾ������B
		if (Status == STATUS::UP) {

			m_angle = DirectX::XM_2PI;

		}
		// ���̃X�e�[�^�X������������B
		else if (Status == STATUS::DOWN) {

			m_angle = 0.0f;

		}

	}

	m_nextStatus = Status;

	m_easingTimer = 0.0f;

}

void TitleUI::TransUp() {

	/*===== ��ɑJ�ڂ���ꍇ =====*/

	// ���݂��ゾ������B
	if (m_status == STATUS::UP) {

		Trans(STATUS::DOWN);

	}
	else if (m_status == STATUS::MIDDLE) {

		Trans(STATUS::UP);

	}
	else if (m_status == STATUS::DOWN) {

		Trans(STATUS::MIDDLE);

	}

}

void TitleUI::TransDown() {

	/*===== ���ɑJ�ڂ���ꍇ =====*/

	// ���݂��ゾ������B
	if (m_status == STATUS::UP) {

		Trans(STATUS::MIDDLE);

	}
	else if (m_status == STATUS::MIDDLE) {

		Trans(STATUS::DOWN);

	}
	else if (m_status == STATUS::DOWN) {

		Trans(STATUS::UP);

	}

}