#include "GameStartTimerUI.h"
#include "Font.h"
#include"AudioApp.h"

GameStartTimerUI::GameStartTimerUI()
{

	/*===== �R���X�g���N�^ =====*/
	m_countUpSE = AudioApp::Instance()->LoadAudio("resource/user/sound/countUp.wav");
	m_countFinishSE = AudioApp::Instance()->LoadAudio("resource/user/sound/countFinish.wav");

	// �e�N���X�𐶐��B
	m_timer[0] = std::make_shared<GameStartTimer>(Font::Instance()->m_stripeFont[3], false, m_countUpSE);
	m_timer[1] = std::make_shared<GameStartTimer>(Font::Instance()->m_stripeFont[2], false, m_countUpSE);
	m_timer[2] = std::make_shared<GameStartTimer>(Font::Instance()->m_stripeFont[1], false, m_countUpSE);
	m_timer[3] = std::make_shared<GameStartTimer>(D3D12App::Instance()->GenerateTextureBuffer("resource/user/go.png"),
		true,m_countFinishSE);

	for (auto& index : m_timer) {

		index->Init();

	}

	m_countTimer = 0;
	m_isActive = false;
	m_isStartGo = false;
}

void GameStartTimerUI::Init()
{

	/*===== ���������� =====*/

	for (auto& index : m_timer) {

		index->Init();

	}

	m_countTimer = 0;
	m_isActive = false;
	m_isStartGo = false;
	m_startFlag = false;

}

void GameStartTimerUI::Update(const Vec2<float>& OffsetPos, const float& AddEasingTimer)
{

	/*===== �`�揈�� =====*/

	if (!m_isActive) return;

	// UI��\������x���ł���^�C�}�[���X�V�B
	++m_countTimer;
	if (COUNT_TIMER <= m_countTimer) {

		m_countTimer = 0;

		// �^�C�}�[�𐶐��B
		for (auto& index : m_timer) {

			if (index->GetIsActive()) continue;

			index->Start();

			break;

		}

	}

	// 3�Ԗڂ̃e�N�X�`��(GO�̉摜)���X�^�[�g�����Ă�����B
	if (m_timer[3]->GetIsStart()) {

		m_isStartGo = true;

	}

	// GO���J�n���Ă��āAGO�̃t���O���܂�Ă����炱�̃N���X�͏I���B
	if (m_isStartGo && m_timer[3]->GetIsEnd()) {

		m_startFlag = true;

	}

	// UI�̍X�V�����B
	for (auto& index : m_timer) {

		index->Update(OffsetPos, AddEasingTimer);

	}

}

void GameStartTimerUI::Draw()
{

	/*===== �`�揈�� =====*/

	if (!m_isActive) return;

	for (auto& index : m_timer) {

		index->Draw();

	}

}

void GameStartTimerUI::Start()
{

	/*===== �J�n���� =====*/

	Init();
	m_isActive = true;

}

GameStartTimer::GameStartTimer(std::shared_ptr<TextureBuffer> TexBuffer, const bool& IsGoTexture, const int& SE)
	:m_se(SE)
{

	/*===== �R���X�g���N�^ =====*/

	m_timerTexture = TexBuffer;
	m_easingTimer = 0;
	m_isActive = false;
	m_isExit = false;
	m_isAppear = false;
	m_timerPos = Vec2<float>();
	m_isGoTexture = IsGoTexture;
	exitTransitionTimer = 0;

}

void GameStartTimer::Init()
{

	/*===== ���������� =====*/

	m_easingTimer = 0;
	m_isActive = false;
	m_isExit = false;
	m_isAppear = false;
	m_timerPos = Vec2<float>();
	exitTransitionTimer = 0;

}

void GameStartTimer::Update(const Vec2<float>& OffsetPos, const float& AddEasingTimer)
{

	/*===== �X�V���� =====*/

	if (!m_isActive) return;

	// �ޏo����������B
	if (m_isExit) {

		// �C�[�W���O�ʂ����߂�B
		float easingAmount = KuroMath::Ease(In, Cubic, m_easingTimer, 0.0f, 1.0f);

		// ���W��ݒ�B
		m_timerPos = (MIDDLE_POS + OffsetPos) + (EXIT_POS - (MIDDLE_POS + OffsetPos)) * easingAmount;

	}

	// �o������������B
	else if (m_isAppear) {

		// �C�[�W���O�ʂ����߂�B
		float easingAmount = KuroMath::Ease(Out, Cubic, m_easingTimer, 0.0f, 1.0f);

		// ���W��ݒ�B
		m_timerPos = APPEAR_POS + ((MIDDLE_POS + OffsetPos) - APPEAR_POS) * easingAmount;

	}



	m_easingTimer += AddEasingTimer;
	if (1.0f <= m_easingTimer) {

		m_easingTimer = 1.0f;

		if (m_isExit) {

		}

		// �o������������B
		else if (m_isAppear) {

			// Exit�ɑJ�ڂ���܂ł̃^�C�}�[���X�V�B
			++exitTransitionTimer;
			if (EXIT_TRANSITION_TIMER <= exitTransitionTimer) {

				m_isExit = true;
				m_easingTimer = 0;

			}

		}

	}

}

#include "DrawFunc2D.h"
void GameStartTimer::Draw()
{

	/*===== �`�揈�� =====*/

	if (!m_isActive) return;

	// �`�悷��T�C�Y�����߂�B
	Vec2<float> drawScale = Vec2<float>(DEF_NUM_SCALE, DEF_NUM_SCALE);
	if (m_isGoTexture) {

		drawScale = Vec2<float>(DEF_GO_SCALE, DEF_GO_SCALE);

	}

	DrawFunc2D::DrawRotaGraph2D(m_timerPos, drawScale, 0, m_timerTexture);

}

void GameStartTimer::Start()
{

	/*===== �o������ =====*/

	m_isActive = true;
	m_isExit = false;
	m_isAppear = true;
	m_easingTimer = 0;
	AudioApp::Instance()->PlayWaveDelay(m_se);

}
