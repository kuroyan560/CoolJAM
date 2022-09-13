#include "TutorialText.h"

TutorialText::TutorialText(std::string File) {

	/*===== �R���X�g���N�^ =====*/

	m_isAppear = false;
	m_isExit = false;
	m_isActive = false;
	m_isFinish = false;
	m_pos = START_POS;
	m_textureEasingTimer = 0;
	m_appearNextDelay = 0;

	m_texture = D3D12App::Instance()->GenerateTextureBuffer(File);

}

void TutorialText::Init() {

	/*===== ���������� =====*/

	m_isAppear = false;
	m_isExit = false;
	m_isActive = false;
	m_isFinish = false;
	m_pos = START_POS;
	m_appearNextDelay = 0;

}

void TutorialText::Update() {

	/*===== �X�V���� =====*/

	// �ޏo����������B
	if (m_isExit) {

		// �C�[�W���O�ʂ����߂�B
		float easingAmount = KuroMath::Ease(InOut, Cubic, m_textureEasingTimer, 0.0f, 1.0f);

		// ���W�����߂�B
		m_pos = MIDDLE_POS + (END_POS - MIDDLE_POS) * easingAmount;

	}
	// �o������������B
	else if (m_isAppear) {

		// �C�[�W���O�ʂ����߂�B
		float easingAmount = KuroMath::Ease(InOut, Cubic, m_textureEasingTimer, 0.0f, 1.0f);

		// ���W�����߂�B
		m_pos = START_POS + (MIDDLE_POS - START_POS) * easingAmount;

	}

	// �C�[�W���O�̃^�C�}�[���X�V����B
	m_textureEasingTimer += ADD_TEXTURE_EASING_TIMER;
	if (1.0f <= m_textureEasingTimer) {

		m_textureEasingTimer = 1.0f;

		//���݂��ޏo���������珉��������B
		if (m_isExit) {

			m_isFinish = true;

		}

		// ���݂��o������������B
		if (m_isAppear) {

			++m_appearNextDelay;
			if (APPEAR_NEXT_DELAY <= m_appearNextDelay) {

				m_appearNextDelay = APPEAR_NEXT_DELAY;

			}

		}

	}

}

#include "DrawFunc2D.h"
void TutorialText::Draw() {

	/*===== �`�揈�� =====*/

	DrawFunc2D::DrawRotaGraph2D(m_pos, Vec2<float>(1.8f, 1.8f), 0.0f, m_texture);

}

void TutorialText::Activate() {

	/*===== �L���� =====*/

	m_isAppear = true;
	m_isActive = true;
	m_textureEasingTimer = 0.0f;

}

void TutorialText::Exit() {

	m_isExit = true;
	m_textureEasingTimer = 0.0f;

}