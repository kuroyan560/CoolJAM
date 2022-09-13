#include "TutorialText.h"

TutorialText::TutorialText(std::string File) {

	/*===== コンストラクタ =====*/

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

	/*===== 初期化処理 =====*/

	m_isAppear = false;
	m_isExit = false;
	m_isActive = false;
	m_isFinish = false;
	m_pos = START_POS;
	m_appearNextDelay = 0;

}

void TutorialText::Update() {

	/*===== 更新処理 =====*/

	// 退出中だったら。
	if (m_isExit) {

		// イージング量を求める。
		float easingAmount = KuroMath::Ease(InOut, Cubic, m_textureEasingTimer, 0.0f, 1.0f);

		// 座標を求める。
		m_pos = MIDDLE_POS + (END_POS - MIDDLE_POS) * easingAmount;

	}
	// 出現中だったら。
	else if (m_isAppear) {

		// イージング量を求める。
		float easingAmount = KuroMath::Ease(InOut, Cubic, m_textureEasingTimer, 0.0f, 1.0f);

		// 座標を求める。
		m_pos = START_POS + (MIDDLE_POS - START_POS) * easingAmount;

	}

	// イージングのタイマーを更新する。
	m_textureEasingTimer += ADD_TEXTURE_EASING_TIMER;
	if (1.0f <= m_textureEasingTimer) {

		m_textureEasingTimer = 1.0f;

		//現在が退出中だったら初期化する。
		if (m_isExit) {

			m_isFinish = true;

		}

		// 現在が出現中だったら。
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

	/*===== 描画処理 =====*/

	DrawFunc2D::DrawRotaGraph2D(m_pos, Vec2<float>(1.8f, 1.8f), 0.0f, m_texture);

}

void TutorialText::Activate() {

	/*===== 有効化 =====*/

	m_isAppear = true;
	m_isActive = true;
	m_textureEasingTimer = 0.0f;

}

void TutorialText::Exit() {

	m_isExit = true;
	m_textureEasingTimer = 0.0f;

}