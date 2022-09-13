#include "TutorialHexagon.h"

TutorialHexagon::TutorialHexagon(std::string File) {

	/*===== �R���X�g���N�^ =====*/

	m_hexagonTex = D3D12App::Instance()->GenerateTextureBuffer(File);

	m_easingTimer = 1.0f;
	m_rotate = 0;
	m_scale = 0;
	m_alpha = 0;

	m_isAppear = false;
	m_isExit = false;
	m_isNormal = false;
	m_isExp = false;

}

void TutorialHexagon::Init() {

	/*===== ���������� =====*/

	m_easingTimer = 1.0f;
	m_rotate = 0;
	m_scale = 0;
	m_alpha = 0;

	m_isAppear = false;
	m_isExit = true;
	m_isNormal = false;
	m_isExp = false;

}

void TutorialHexagon::Update() {

	/*===== �X�V���� =====*/

	// �o����Ԃ�������B
	if (m_isAppear) {

		// �C�[�W���O�ʂ����߂�B
		float easingAmount = KuroMath::Ease(InOut, Exp, m_easingTimer, 0.0f, 1.0f);

		// �X�P�[���Ɖ�]�����߂�B
		m_scale = DEF_SCALE * easingAmount;
		m_rotate = 0.0f;
		m_alpha = 1.0f * easingAmount;

	}

	// �ޏo����������
	if (m_isExit) {

		// �X�P�[���Ɖ�]�����߂�B
		if (m_scale != 0) {
			m_scale -= m_scale / 5.0f;
		}
		if (m_rotate != 0) {
			m_rotate -= m_rotate / 5.0f;
		}
		if (m_alpha != 0) {
			m_alpha -= m_alpha / 5.0f;
		}

	}

	// �ʏ��Ԃ�������B
	if (m_isNormal) {

		// �g�咆��������B
		if (m_isExp) {

			// �C�[�W���O�ʂ����߂�B
			float easingAmount = KuroMath::Ease(In, Exp, m_easingTimer, 0.0f, 0.5f);

			// �X�P�[�������߂�B
			m_scale = DEF_SCALE + (EXP_SCALE - DEF_SCALE) * easingAmount;
			m_rotate = EASE_ROTATE * easingAmount;

		}
		// �k������������B
		else {

			// �C�[�W���O�ʂ����߂�B
			float easingAmount = KuroMath::Ease(Out, Exp, m_easingTimer, 0.5f, 1.0f);

			// �X�P�[�������߂�B
			m_scale = EXP_SCALE + (DEF_SCALE - EXP_SCALE) * easingAmount;
			m_rotate = EASE_ROTATE * easingAmount;

		}

	}

	// �^�C�}�[������l�𒴂��Ă�����B
	m_easingTimer += ADD_EASING_TIMER;
	if (1.0f <= m_easingTimer) {

		// �o������������^�C�}�[�����������āA�ʏ��ԂɈڍs����B
		if (m_isAppear) {

			m_easingTimer = 0;
			m_isNormal = true;
			m_isExp = true;
			m_isAppear = false;

		}
		// �ʏ��Ԃ�������^�C�}�[������������B
		else if (m_isNormal) {

			m_easingTimer = 0;

			// �t���O�𔽓]������B
			m_isExp = m_isExp ? false : true;

		}
		// ���ł��イ��������B
		else if (m_isExit) {

			m_easingTimer = 1.0f;

		}

	}

}

#include "DrawFunc2D.h"
void TutorialHexagon::Draw() {

	/*===== �`�揈�� =====*/

	// �A���t�@�����ȉ���������`�悵�Ȃ��B
	if (0.05f < m_alpha) {
		DrawFunc2D::DrawRotaGraph2D(DRAW_POS, Vec2<float>(m_scale, m_scale), m_rotate, m_hexagonTex, m_alpha);
	}

}

void TutorialHexagon::Appear() {

	/*===== �o������ =====*/

	m_isExit = false;
	m_isAppear = true;
	m_isNormal = false;
	m_easingTimer = 0;

}

void TutorialHexagon::Exit() {

	/*===== �ޏo���� =====*/

	m_isExit = true;
	m_isAppear = false;
	m_isNormal = false;
	m_isExp = false;

}