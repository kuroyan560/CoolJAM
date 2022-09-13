#include "TutorialNice.h"

TutorialNice::TutorialNice(std::string File) {

	/*===== �R���X�g���N�^ =====*/

	m_niceTexture = D3D12App::Instance()->GenerateTextureBuffer(File);

	m_easingTimer = 0.0f;
	m_scale = 0;
	m_alpha = 0;
	m_exitTimer = 0;

	m_isAppear = false;
	m_isExit = false;
	m_isActive = false;

}

void TutorialNice::Init() {

	/*===== ���������� =====*/

	m_easingTimer = 0.0f;
	m_scale = 0;
	m_alpha = 0;
	m_exitTimer = 0;

	m_isAppear = false;
	m_isExit = false;
	m_isActive = false;

}

void TutorialNice::Update() {

	/*===== �X�V���� =====*/

	if (!m_isActive) return;

	// �o������������B
	if (m_isAppear) {

		// �C�[�W���O�ʂ����߂�B
		float easingAmount = KuroMath::Ease(In, Exp, m_easingTimer, 0.0f, 1.0f);

		// �X�P�[���ƃA���t�@�l��ݒ�B
		m_scale = EXP_SCALE + (DEF_SCALE - EXP_SCALE) * easingAmount;
		m_alpha = 1.0f * easingAmount;

		// �C�[�W���O�^�C�}�[���X�V�B
		m_easingTimer += ADD_EASING_TIMER;
		if (1.0f <= m_easingTimer) {

			m_easingTimer = 1.0f;

			++m_exitTimer;
			if (EXIT_TIMER <= m_exitTimer) {

				m_isAppear = false;
				m_isExit = true;

			}

		}

	}
	// ���Œ���������B
	else if (m_isExit) {

		// �A���t�@�l��������B
		m_alpha -= m_alpha / 10.0f;
		if (m_alpha <= 0.05f) {

			m_alpha = 0.0f;
			m_isActive = false;

		}

	}

}

#include "DrawFunc2D.h"
void TutorialNice::Draw() {

	/*===== �`�揈�� =====*/

	if (!m_isActive) return;

	// �A���t�@�����ȉ���������`�悵�Ȃ��B
	DrawFunc2D::DrawRotaGraph2D(DRAW_POS, Vec2<float>(m_scale, m_scale), 0.0f, m_niceTexture, m_alpha);

}

void TutorialNice::Appear() {

	/*===== �o������ =====*/

	m_isExit = false;
	m_isAppear = true;
	m_isActive = true;
	m_easingTimer = 0;
	m_exitTimer = 0;

}