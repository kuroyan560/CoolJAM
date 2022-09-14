#include "GameStartTimerUI.h"
#include "Font.h"
#include"AudioApp.h"

GameStartTimerUI::GameStartTimerUI()
{

	/*===== コンストラクタ =====*/
	m_countUpSE = AudioApp::Instance()->LoadAudio("resource/user/sound/countUp.wav");
	m_countFinishSE = AudioApp::Instance()->LoadAudio("resource/user/sound/countFinish.wav");

	// 各クラスを生成。
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

	/*===== 初期化処理 =====*/

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

	/*===== 描画処理 =====*/

	if (!m_isActive) return;

	// UIを表示する遅延であるタイマーを更新。
	++m_countTimer;
	if (COUNT_TIMER <= m_countTimer) {

		m_countTimer = 0;

		// タイマーを生成。
		for (auto& index : m_timer) {

			if (index->GetIsActive()) continue;

			index->Start();

			break;

		}

	}

	// 3番目のテクスチャ(GOの画像)がスタートさせていたら。
	if (m_timer[3]->GetIsStart()) {

		m_isStartGo = true;

	}

	// GOを開始していて、GOのフラグが折れていたらこのクラスは終わり。
	if (m_isStartGo && m_timer[3]->GetIsEnd()) {

		m_startFlag = true;

	}

	// UIの更新処理。
	for (auto& index : m_timer) {

		index->Update(OffsetPos, AddEasingTimer);

	}

}

void GameStartTimerUI::Draw()
{

	/*===== 描画処理 =====*/

	if (!m_isActive) return;

	for (auto& index : m_timer) {

		index->Draw();

	}

}

void GameStartTimerUI::Start()
{

	/*===== 開始処理 =====*/

	Init();
	m_isActive = true;

}

GameStartTimer::GameStartTimer(std::shared_ptr<TextureBuffer> TexBuffer, const bool& IsGoTexture, const int& SE)
	:m_se(SE)
{

	/*===== コンストラクタ =====*/

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

	/*===== 初期化処理 =====*/

	m_easingTimer = 0;
	m_isActive = false;
	m_isExit = false;
	m_isAppear = false;
	m_timerPos = Vec2<float>();
	exitTransitionTimer = 0;

}

void GameStartTimer::Update(const Vec2<float>& OffsetPos, const float& AddEasingTimer)
{

	/*===== 更新処理 =====*/

	if (!m_isActive) return;

	// 退出中だったら。
	if (m_isExit) {

		// イージング量を求める。
		float easingAmount = KuroMath::Ease(In, Cubic, m_easingTimer, 0.0f, 1.0f);

		// 座標を設定。
		m_timerPos = (MIDDLE_POS + OffsetPos) + (EXIT_POS - (MIDDLE_POS + OffsetPos)) * easingAmount;

	}

	// 出現中だったら。
	else if (m_isAppear) {

		// イージング量を求める。
		float easingAmount = KuroMath::Ease(Out, Cubic, m_easingTimer, 0.0f, 1.0f);

		// 座標を設定。
		m_timerPos = APPEAR_POS + ((MIDDLE_POS + OffsetPos) - APPEAR_POS) * easingAmount;

	}



	m_easingTimer += AddEasingTimer;
	if (1.0f <= m_easingTimer) {

		m_easingTimer = 1.0f;

		if (m_isExit) {

		}

		// 出現中だったら。
		else if (m_isAppear) {

			// Exitに遷移するまでのタイマーを更新。
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

	/*===== 描画処理 =====*/

	if (!m_isActive) return;

	// 描画するサイズを求める。
	Vec2<float> drawScale = Vec2<float>(DEF_NUM_SCALE, DEF_NUM_SCALE);
	if (m_isGoTexture) {

		drawScale = Vec2<float>(DEF_GO_SCALE, DEF_GO_SCALE);

	}

	DrawFunc2D::DrawRotaGraph2D(m_timerPos, drawScale, 0, m_timerTexture);

}

void GameStartTimer::Start()
{

	/*===== 出現処理 =====*/

	m_isActive = true;
	m_isExit = false;
	m_isAppear = true;
	m_easingTimer = 0;
	AudioApp::Instance()->PlayWaveDelay(m_se);

}
