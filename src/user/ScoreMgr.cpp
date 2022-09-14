#include "ScoreMgr.h"
#include "Font.h"
#include "KuroEngine.h"
#include "KuroFunc.h"
#include "SlowMgr.h"

void ScoreMgr::Init()
{

	/*===== ���������� =====*/

	m_prevScore = 0;
	m_score = 0;
	m_addScoreRegister = 0;
	m_scoreScale = DEF_SCALE;
	m_ScoreTexture = D3D12App::Instance()->GenerateTextureBuffer("resource/user/score.png");
	m_easingTimer = 0.0f;
	m_isActive = false;
}

void ScoreMgr::Update(const Vec2<float>& Offset, const Vec2<float>& CenterOffset, const float& AddEasingTimer)
{

	/*===== �X�V���� =====*/

	// �X�R�A�ɉ��Z����ʂ����߂�B
	float addScorePoint = 0;

	// �ꎟ�ۑ������X�R�A��0���ゾ������B
	if (0 < m_addScoreRegister) {
		addScorePoint = m_addScoreRegister / 2.0f * SlowMgr::Instance()->m_slow;
	}

	// �X�R�A�����Z����B
	m_prevScore = m_score;
	m_score += addScorePoint * SlowMgr::Instance()->m_slow;
	m_addScoreRegister -= addScorePoint * SlowMgr::Instance()->m_slow;


	// �������ȉ���UI�̘b�B

	if (!m_isActive) return;

	// �o������������B
	if (m_isAppear) {

		// �C�[�W���O�ʂ����߂�B
		float easingAmount = KuroMath::Ease(Out, Cubic, m_easingTimer, 0.0f, 1.0f);

		// ���W��ݒ�B
		m_pos = APPEAR_POS + ((MIDDLE_POS + Offset) - APPEAR_POS) * easingAmount;

	}
	// �ޏo����������B
	else if (m_isExit) {

		// �C�[�W���O�ʂ����߂�B
		float easingAmount = KuroMath::Ease(In, Cubic, m_easingTimer, 0.0f, 1.0f);

		// ��Ԍ��̍��W�B
		Vec2<float> basePos = MIDDLE_POS;
		if (m_isCenter) {

			basePos = CENTER_POS + Offset + CenterOffset - Vec2<float>(46.0f, 15.0f);

		}

		// ���W��ݒ�B
		m_pos = (basePos + Offset) + (EXIT_POS - (basePos + Offset)) * easingAmount;
		m_pos.y += 30.0f;
	}
	// �����ڍs��Ԃ�������B
	else if (m_isCenter) {

		// �C�[�W���O�ʂ����߂�B
		float easingAmount = KuroMath::Ease(InOut, Cubic, m_easingTimer, 0.0f, 1.0f);

		// ���W��ݒ�B
		m_pos = (MIDDLE_POS + Offset) + ((CENTER_POS + Offset + CenterOffset) - (MIDDLE_POS + Offset)) * easingAmount;

	}


	m_easingTimer += AddEasingTimer * SlowMgr::Instance()->m_slow;
	if (1.0f <= m_easingTimer) {

		m_easingTimer = 1.0f;

	}

	// �O�t���[���ƒl���ς���Ă�����X�P�[����ς���B
	if (m_score != m_prevScore) {

		m_scoreScale = 0.8f;

	}
	else {

		m_scoreScale += (DEF_SCALE - m_scoreScale) / 3.0f;

	}


}

#include "DrawFunc2D.h"
void ScoreMgr::Draw(const AdjData& OFFSET_DATA)
{

	/*===== �`�揈�� =====*/

	if (!m_isActive) return;

	// �X�R�A�̉摜��`�悷��B
	DrawFunc2D::DrawRotaGraph2D(m_pos + Vec2<float>(-280.0f, -100.0f) + OFFSET_DATA.m_stringPos, Vec2<float>(0.9f, 0.9f) + Vec2<float>(OFFSET_DATA.m_stringSize, OFFSET_DATA.m_stringSize), 0.0f, m_ScoreTexture);


	// ���������߂�B
	int nowScoreLength = static_cast<int>(std::to_string(m_score).size());
	if (nowScoreLength <= 0)  return;


	float scale = m_scoreScale + OFFSET_DATA.m_numSize;

	// �J�n�ʒu�����߂�B
	Vec2<float> startPos = m_pos + OFFSET_DATA.m_numPos;
	startPos.x -= FONT_OFFSET * (nowScoreLength / 2.0f);
	for (int index = 0; index < nowScoreLength; ++index) {

		DrawFunc2D::DrawRotaGraph2D(startPos + Vec2<float>((FONT_OFFSET + 20.0f) * index, -20.0f), Vec2<float>(scale, scale), 0.0f, Font::Instance()->m_stripeFont[KuroFunc::GetSpecifiedDigitNum(m_score, nowScoreLength - index - 1)]);

	}

}

void ScoreMgr::Appear()
{

	/*===== �o������ =====*/

	m_isActive = true;
	m_isAppear = true;
	m_isExit = false;
	m_isCenter = false;
	m_easingTimer = 0;

}

void ScoreMgr::Exit()
{

	/*===== �o������ =====*/

	m_isActive = true;
	m_isAppear = false;
	m_isExit = true;
	m_easingTimer = 0;

}

void ScoreMgr::Center()
{

	/*===== �����ڍs���� =====*/

	m_isActive = true;
	m_isAppear = false;
	m_isExit = false;
	m_isCenter = true;
	m_easingTimer = 0;

}

void ScoreMgr::AddScore(const int& Score)
{

	/*===== �ꎟ�ۑ��X�R�A�����Z���� =====*/

	m_addScoreRegister += Score;

}

void ScoreMgr::GameFinish()
{

	/*===== �ۑ����Ă���X�R�A����C�ɑ������ =====*/

	m_score = m_addScoreRegister;

}