#include "TutorialNice.h"

TutorialNice::TutorialNice(std::string File) {

	/*===== コンストラクタ =====*/

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

	/*===== 初期化処理 =====*/

	m_easingTimer = 0.0f;
	m_scale = 0;
	m_alpha = 0;
	m_exitTimer = 0;

	m_isAppear = false;
	m_isExit = false;
	m_isActive = false;

}

void TutorialNice::Update() {

	/*===== 更新処理 =====*/

	if (!m_isActive) return;

	// 出現中だったら。
	if (m_isAppear) {

		// イージング量を求める。
		float easingAmount = KuroMath::Ease(In, Exp, m_easingTimer, 0.0f, 1.0f);

		// スケールとアルファ値を設定。
		m_scale = EXP_SCALE + (DEF_SCALE - EXP_SCALE) * easingAmount;
		m_alpha = 1.0f * easingAmount;

		// イージングタイマーを更新。
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
	// 消滅中だったら。
	else if (m_isExit) {

		// アルファ値を下げる。
		m_alpha -= m_alpha / 10.0f;
		if (m_alpha <= 0.05f) {

			m_alpha = 0.0f;
			m_isActive = false;

		}

	}

}

#include "DrawFunc2D.h"
void TutorialNice::Draw() {

	/*===== 描画処理 =====*/

	if (!m_isActive) return;

	// アルファが一定以下だったら描画しない。
	DrawFunc2D::DrawRotaGraph2D(DRAW_POS, Vec2<float>(m_scale, m_scale), 0.0f, m_niceTexture, m_alpha);

}

void TutorialNice::Appear() {

	/*===== 出現処理 =====*/

	m_isExit = false;
	m_isAppear = true;
	m_isActive = true;
	m_easingTimer = 0;
	m_exitTimer = 0;

}