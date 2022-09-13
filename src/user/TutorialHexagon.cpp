#include "TutorialHexagon.h"

TutorialHexagon::TutorialHexagon(std::string File) {

	/*===== コンストラクタ =====*/

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

	/*===== 初期化処理 =====*/

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

	/*===== 更新処理 =====*/

	// 出現状態だったら。
	if (m_isAppear) {

		// イージング量を求める。
		float easingAmount = KuroMath::Ease(InOut, Exp, m_easingTimer, 0.0f, 1.0f);

		// スケールと回転を求める。
		m_scale = DEF_SCALE * easingAmount;
		m_rotate = 0.0f;
		m_alpha = 1.0f * easingAmount;

	}

	// 退出中だったら
	if (m_isExit) {

		// スケールと回転を求める。
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

	// 通常状態だったら。
	if (m_isNormal) {

		// 拡大中だったら。
		if (m_isExp) {

			// イージング量を求める。
			float easingAmount = KuroMath::Ease(In, Exp, m_easingTimer, 0.0f, 0.5f);

			// スケールを求める。
			m_scale = DEF_SCALE + (EXP_SCALE - DEF_SCALE) * easingAmount;
			m_rotate = EASE_ROTATE * easingAmount;

		}
		// 縮小中だったら。
		else {

			// イージング量を求める。
			float easingAmount = KuroMath::Ease(Out, Exp, m_easingTimer, 0.5f, 1.0f);

			// スケールを求める。
			m_scale = EXP_SCALE + (DEF_SCALE - EXP_SCALE) * easingAmount;
			m_rotate = EASE_ROTATE * easingAmount;

		}

	}

	// タイマーが既定値を超えていたら。
	m_easingTimer += ADD_EASING_TIMER;
	if (1.0f <= m_easingTimer) {

		// 出現中だったらタイマーを初期化して、通常状態に移行する。
		if (m_isAppear) {

			m_easingTimer = 0;
			m_isNormal = true;
			m_isExp = true;
			m_isAppear = false;

		}
		// 通常状態だったらタイマーを初期化する。
		else if (m_isNormal) {

			m_easingTimer = 0;

			// フラグを反転させる。
			m_isExp = m_isExp ? false : true;

		}
		// 消滅ちゅうだったら。
		else if (m_isExit) {

			m_easingTimer = 1.0f;

		}

	}

}

#include "DrawFunc2D.h"
void TutorialHexagon::Draw() {

	/*===== 描画処理 =====*/

	// アルファが一定以下だったら描画しない。
	if (0.05f < m_alpha) {
		DrawFunc2D::DrawRotaGraph2D(DRAW_POS, Vec2<float>(m_scale, m_scale), m_rotate, m_hexagonTex, m_alpha);
	}

}

void TutorialHexagon::Appear() {

	/*===== 出現処理 =====*/

	m_isExit = false;
	m_isAppear = true;
	m_isNormal = false;
	m_easingTimer = 0;

}

void TutorialHexagon::Exit() {

	/*===== 退出処理 =====*/

	m_isExit = true;
	m_isAppear = false;
	m_isNormal = false;
	m_isExp = false;

}