#include "WaveUI.h"

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

void WaveUI::Init()
{

	/*===== 初期化処理 =====*/

	m_easingTimer = 0;
	m_isActive = false;
	m_isExit = false;
	m_isAppear = false;
	m_wavePos = Vec2<float>();

}

void WaveUI::Update()
{

	/*===== 更新処理 =====*/

	if (!m_isActive) return;

	// 出現中だったら。
	if (m_isAppear) {

		// イージング量を求める。
		float easingAmount = KuroMath::Ease(Out, Cubic, m_easingTimer, 0.0f, 1.0f);

		// 座標を設定。
		m_wavePos = APPEAR_POS + (MIDDLE_POS - APPEAR_POS) * easingAmount;

	}

	// 退出中だったら。
	if (m_isExit) {

		// イージング量を求める。
		float easingAmount = KuroMath::Ease(In, Cubic, m_easingTimer, 0.0f, 1.0f);

		// 座標を設定。
		m_wavePos = MIDDLE_POS + (EXIT_POS - MIDDLE_POS) * easingAmount;

	}

	m_easingTimer += ADD_EASING_TIMER;
	if (1.0f <= m_easingTimer) {

		m_easingTimer = 1.0f;

	}

}

#include "DrawFunc2D.h"
void WaveUI::Draw()
{

	/*===== 描画処理 =====*/

	if (!m_isActive) return;

	DrawFunc2D::DrawRotaGraph2D(m_wavePos + Vec2<float>(-200.0f, 0.0f), Vec2<float>(1.0f, 1.0f), 0, m_waveTexture);

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
