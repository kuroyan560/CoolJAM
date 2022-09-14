#include "ScoreMgr.h"
#include "Font.h"
#include "KuroEngine.h"
#include "KuroFunc.h"
#include "SlowMgr.h"

void ScoreMgr::Init()
{

	/*===== 初期化処理 =====*/

	m_prevScore = 0;
	m_score = 0;
	m_addScoreRegister = 0;
	m_scoreScale = DEF_SCALE;
	m_ScoreTexture = D3D12App::Instance()->GenerateTextureBuffer("resource/user/score.png");

}

void ScoreMgr::Update(const Vec2<float>& Offset, const Vec2<float>& CenterOffset, const float& AddEasingTimer)
{

	/*===== 更新処理 =====*/

	// スコアに加算する量を決める。
	float addScorePoint = 0;

	// 一次保存したスコアが0より上だったら。
	if (0 < m_addScoreRegister) {
		addScorePoint = m_addScoreRegister / 2.0f * SlowMgr::Instance()->m_slow;
	}

	// スコアを加算する。
	m_prevScore = m_score;
	m_score += addScorePoint * SlowMgr::Instance()->m_slow;
	m_addScoreRegister -= addScorePoint * SlowMgr::Instance()->m_slow;


	// ここより以下はUIの話。

	if (!m_isActive) return;

	// 出現中だったら。
	if (m_isAppear) {

		// イージング量を求める。
		float easingAmount = KuroMath::Ease(Out, Cubic, m_easingTimer, 0.0f, 1.0f);

		// 座標を設定。
		m_pos = APPEAR_POS + ((MIDDLE_POS + Offset) - APPEAR_POS) * easingAmount;

	}
	// 退出中だったら。
	else if (m_isExit) {

		// イージング量を求める。
		float easingAmount = KuroMath::Ease(In, Cubic, m_easingTimer, 0.0f, 1.0f);

		// 補間元の座標。
		Vec2<float> basePos = MIDDLE_POS;
		if (m_isCenter) {

			basePos = CENTER_POS + Offset + CenterOffset;

		}

		// 座標を設定。
		m_pos = (basePos + Offset) + (EXIT_POS - (basePos + Offset)) * easingAmount;

	}
	// 中央移行状態だったら。
	else if (m_isCenter) {

		// イージング量を求める。
		float easingAmount = KuroMath::Ease(InOut, Cubic, m_easingTimer, 0.0f, 1.0f);

		// 座標を設定。
		m_pos = (MIDDLE_POS + Offset) + ((CENTER_POS + Offset + CenterOffset) - (MIDDLE_POS + Offset)) * easingAmount;

	}


	m_easingTimer += AddEasingTimer * SlowMgr::Instance()->m_slow;
	if (1.0f <= m_easingTimer) {

		m_easingTimer = 1.0f;

	}

	// 前フレームと値が変わっていたらスケールを変える。
	if (m_score != m_prevScore) {

		m_scoreScale = 0.8f;

	}
	else {

		m_scoreScale += (DEF_SCALE - m_scoreScale) / 3.0f;

	}


}

#include "DrawFunc2D.h"
void ScoreMgr::Draw()
{

	/*===== 描画処理 =====*/

	if (!m_isActive) return;

	// スコアの画像を描画する。
	DrawFunc2D::DrawRotaGraph2D(m_pos + Vec2<float>(-280.0f, -100.0f), Vec2<float>(0.9f, 0.9f), 0.0f, m_ScoreTexture);


	// 桁数を求める。
	int nowScoreLength = static_cast<int>(std::to_string(m_score).size());
	if (nowScoreLength <= 0)  return;

	// 開始位置を求める。
	Vec2<float> startPos = m_pos;
	startPos.x -= FONT_OFFSET * (nowScoreLength / 2.0f);
	for (int index = 0; index < nowScoreLength; ++index) {

		DrawFunc2D::DrawRotaGraph2D(startPos + Vec2<float>(FONT_OFFSET * index, 0.0f), Vec2<float>(m_scoreScale, m_scoreScale), 0.0f, Font::Instance()->m_stripeFont[KuroFunc::GetSpecifiedDigitNum(m_score, nowScoreLength - index - 1)]);

	}

}

void ScoreMgr::Appear()
{

	/*===== 出現処理 =====*/

	m_isActive = true;
	m_isAppear = true;
	m_isExit = false;
	m_isCenter = false;
	m_easingTimer = 0;

}

void ScoreMgr::Exit()
{

	/*===== 出現処理 =====*/

	m_isActive = true;
	m_isAppear = false;
	m_isExit = true;
	m_easingTimer = 0;

}

void ScoreMgr::Center()
{

	/*===== 中央移行処理 =====*/

	m_isActive = true;
	m_isAppear = false;
	m_isExit = false;
	m_isCenter = true;
	m_easingTimer = 0;

}

void ScoreMgr::AddScore(const int& Score)
{

	/*===== 一次保存スコアを加算する =====*/

	m_addScoreRegister += Score;

}

void ScoreMgr::GameFinish()
{

	/*===== 保存してあるスコアを一気に代入する =====*/

	m_score = m_addScoreRegister;

}
