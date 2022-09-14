#include "WaveUI.h"
#include "Font.h"
#include "SlowMgr.h"

WaveUI::WaveUI(std::string File)
{

	/*===== コンストラクタ =====*/

	m_waveTexture = D3D12App::Instance()->GenerateTextureBuffer(File);
	m_easingTimer = 0;
	m_isActive = false;
	m_isExit = false;
	m_isAppear = false;
	m_wavePos = Vec2<float>();

}

void WaveUI::Init(const int& MaxWave)
{

	/*===== 初期化処理 =====*/

	m_easingTimer = 0;
	m_isActive = false;
	m_isExit = false;
	m_isAppear = false;
	m_wavePos = Vec2<float>();

	// 最大数が10未満だったら。
	if (MaxWave < 10) {

		m_maxWaveCountIndex[0] = -1;
		m_maxWaveCountIndex[1] = MaxWave;

	}
	else {

		m_maxWaveCountIndex[0] = KuroFunc::GetSpecifiedDigitNum(MaxWave, 1);
		m_maxWaveCountIndex[1] = KuroFunc::GetSpecifiedDigitNum(MaxWave, 0);

	}

	m_nowWaveCountIndex[0] = 0;
	m_nowWaveCountIndex[0] = 0;

}

void WaveUI::Update(const int& NowWave, const Vec2<float>& OffsetPos, const Vec2<float>& CenterOffset, const float& AddEasingTimer)
{

	/*===== 更新処理 =====*/

	if (!m_isActive) return;

	// 出現中だったら。
	if (m_isAppear) {

		// イージング量を求める。
		float easingAmount = KuroMath::Ease(Out, Cubic, m_easingTimer, 0.0f, 1.0f);

		// 座標を設定。
		m_wavePos = APPEAR_POS + ((MIDDLE_POS + OffsetPos) - APPEAR_POS) * easingAmount;

	}
	// 退出中だったら。
	else if (m_isExit) {

		// イージング量を求める。
		float easingAmount = KuroMath::Ease(In, Cubic, m_easingTimer, 0.0f, 1.0f);

		// 補間元の座標。
		Vec2<float> basePos = MIDDLE_POS;
		if (m_isCenter) {

			basePos = CENTER_POS + OffsetPos + CenterOffset;

		}

		// 座標を設定。
		m_wavePos = (basePos + OffsetPos) + (EXIT_POS - (basePos + OffsetPos)) * easingAmount;

	}
	// 中央移行状態だったら。
	else if (m_isCenter) {

		// イージング量を求める。
		float easingAmount = KuroMath::Ease(InOut, Cubic, m_easingTimer, 0.0f, 1.0f);

		// 座標を設定。
		m_wavePos = (MIDDLE_POS + OffsetPos) + ((CENTER_POS + OffsetPos + CenterOffset) - (MIDDLE_POS + OffsetPos)) * easingAmount;

	}

	m_easingTimer += AddEasingTimer * SlowMgr::Instance()->m_slow;
	if (1.0f <= m_easingTimer) {

		m_easingTimer = 1.0f;

	}

	// 最大数が10未満だったら。
	if (NowWave < 10) {

		m_nowWaveCountIndex[0] = -1;
		m_nowWaveCountIndex[1] = NowWave;

	}
	else {

		m_nowWaveCountIndex[0] = KuroFunc::GetSpecifiedDigitNum(NowWave, 1);
		m_nowWaveCountIndex[1] = KuroFunc::GetSpecifiedDigitNum(NowWave, 0);

	}

}

#include "DrawFunc2D.h"
void WaveUI::Draw(const AdjData &OFFSET_DATA)
{

	/*===== 描画処理 =====*/

	if (!m_isActive) return;

	DrawFunc2D::DrawRotaGraph2D(m_wavePos + Vec2<float>(-250.0f, 0.0f)+ OFFSET_DATA.m_stringPos, Vec2<float>(1.0f, 1.0f) + Vec2<float>(OFFSET_DATA.m_stringSize, OFFSET_DATA.m_stringSize), 0, m_waveTexture);


	if (m_nowWaveCountIndex[0] != -1) {
		DrawFunc2D::DrawRotaGraph2D(m_wavePos + Vec2<float>(20.0f, 80.0f)+ OFFSET_DATA.m_numPos, Vec2<float>(1.25f, 1.25f) + Vec2<float>(OFFSET_DATA.m_numSize, OFFSET_DATA.m_numSize), 0, Font::Instance()->m_stripeFont[m_nowWaveCountIndex[0]]);
	}
	if (m_nowWaveCountIndex[1] != -1) {
		DrawFunc2D::DrawRotaGraph2D(m_wavePos + Vec2<float>(170.0f, 80.0f) + OFFSET_DATA.m_numPos, Vec2<float>(1.25f, 1.25f) + Vec2<float>(OFFSET_DATA.m_numSize, OFFSET_DATA.m_numSize), 0, Font::Instance()->m_stripeFont[m_nowWaveCountIndex[1]]);
	}

	DrawFunc2D::DrawRotaGraph2D(m_wavePos + Vec2<float>(300.0f, 120.0f) + OFFSET_DATA.m_numPos, Vec2<float>(0.5f, 0.5f) + Vec2<float>(OFFSET_DATA.m_numSize, OFFSET_DATA.m_numSize), 0, Font::Instance()->m_stripeFont[11]);

	if (m_maxWaveCountIndex[0] != -1) {
		DrawFunc2D::DrawRotaGraph2D(m_wavePos + Vec2<float>(380.0f, 120.0f) + OFFSET_DATA.m_numPos, Vec2<float>(0.5f, 0.5f) + Vec2<float>(OFFSET_DATA.m_numSize, OFFSET_DATA.m_numSize), 0, Font::Instance()->m_stripeFont[m_maxWaveCountIndex[0]]);
	}
	if (m_maxWaveCountIndex[1] != -1) {
		DrawFunc2D::DrawRotaGraph2D(m_wavePos + Vec2<float>(450.0f, 120.0f) + OFFSET_DATA.m_numPos, Vec2<float>(0.5f, 0.5f) + Vec2<float>(OFFSET_DATA.m_numSize, OFFSET_DATA.m_numSize), 0, Font::Instance()->m_stripeFont[m_maxWaveCountIndex[1]]);
	}

}

void WaveUI::Appear()
{

	/*===== 出現処理 =====*/

	m_isActive = true;
	m_isExit = false;
	m_isAppear = true;
	m_easingTimer = 0;

}

void WaveUI::Exit()
{

	/*===== 消滅処理 =====*/

	m_isExit = true;
	m_isAppear = false;
	m_easingTimer = 0;

}

void WaveUI::Center()
{

	/*===== 中央移行処理 =====*/

	m_isActive = true;
	m_isAppear = false;
	m_isExit = false;
	m_isCenter = true;
	m_easingTimer = 0;

}
