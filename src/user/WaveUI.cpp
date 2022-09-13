#include "WaveUI.h"
#include "Font.h"

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

void WaveUI::Update(const int& NowWave, const Vec2<float>& OffsetPos)
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
	if (m_isExit) {

		// イージング量を求める。
		float easingAmount = KuroMath::Ease(In, Cubic, m_easingTimer, 0.0f, 1.0f);

		// 座標を設定。
		m_wavePos = (MIDDLE_POS + OffsetPos) + (EXIT_POS - (MIDDLE_POS + OffsetPos)) * easingAmount;

	}

	m_easingTimer += ADD_EASING_TIMER;
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
void WaveUI::Draw()
{

	/*===== 描画処理 =====*/

	if (!m_isActive) return;

	DrawFunc2D::DrawRotaGraph2D(m_wavePos + Vec2<float>(-200.0f, 0.0f), Vec2<float>(1.0f, 1.0f), 0, m_waveTexture);


	if (m_nowWaveCountIndex[0] != -1) {
		DrawFunc2D::DrawRotaGraph2D(m_wavePos + Vec2<float>(100.0f, 0.0f), Vec2<float>(1.5f, 1.5f), 0, Font::Instance()->m_stripeFont[m_nowWaveCountIndex[0]]);
	}
	if (m_nowWaveCountIndex[1] != -1) {
		DrawFunc2D::DrawRotaGraph2D(m_wavePos + Vec2<float>(200.0f, 0.0f), Vec2<float>(1.5f, 1.5f), 0, Font::Instance()->m_stripeFont[m_nowWaveCountIndex[1]]);
	}

	DrawFunc2D::DrawRotaGraph2D(m_wavePos + Vec2<float>(300.0f, 20.0f), Vec2<float>(1.0f, 1.0f), 0, Font::Instance()->m_stripeFont[11]);

	if (m_maxWaveCountIndex[0] != -1) {
		DrawFunc2D::DrawRotaGraph2D(m_wavePos + Vec2<float>(380.0f, 20.0f), Vec2<float>(1.0f, 1.0f), 0, Font::Instance()->m_stripeFont[m_maxWaveCountIndex[0]]);
	}
	if (m_maxWaveCountIndex[1] != -1) {
		DrawFunc2D::DrawRotaGraph2D(m_wavePos + Vec2<float>(450.0f, 20.0f), Vec2<float>(1.0f, 1.0f), 0, Font::Instance()->m_stripeFont[m_maxWaveCountIndex[1]]);
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
