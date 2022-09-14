#pragma once
#include "Vec.h"
#include "D3D12App.h"
#include "D3D12Data.h"
#include "WinApp.h"
#include <string>
#include <memory>
#include <array>

class GameStartTimer {
	/*===== メンバ変数 =====*/

	std::shared_ptr<TextureBuffer> m_timerTexture;	// タイマーのテクスチャ

	Vec2<float> m_timerPos;

	Vec2<float> m_offsetPos;

	float m_easingTimer;	// イージングのタイマー

	int exitTransitionTimer;	// 中心にとどまる時間。
	const int EXIT_TRANSITION_TIMER = 10;

	bool m_isAppear;		// 出現中か。
	bool m_isExit;			// 消滅中か。
	bool m_isActive;
	bool m_isGoTexture;

	const float DEF_NUM_SCALE = 1.0f;
	const float DEF_GO_SCALE = 1.5f;
	const Vec2<float> MIDDLE_POS = WinApp::Instance()->GetExpandWinCenter();
	const Vec2<float> APPEAR_POS = Vec2<float>(WinApp::Instance()->GetExpandWinCenter().x, WinApp::Instance()->GetExpandWinSize().y + WinApp::Instance()->GetExpandWinCenter().y);
	const Vec2<float> EXIT_POS = Vec2<float>(WinApp::Instance()->GetExpandWinCenter().x, -WinApp::Instance()->GetExpandWinCenter().y);

public:

	/*===== メンバ関数 =====*/

	GameStartTimer(std::shared_ptr<TextureBuffer> TexBuffer, const bool& IsGoTexture);
	void Init();
	void Update(const Vec2<float>& OffsetPos, const float& AddEasingTimer);
	void Draw();

	// 出現している状態か
	bool GetIsActive() { return m_isActive; }
	bool GetIsStart() { return m_isAppear; }
	bool GetIsEnd() { return m_isExit && 1.0f <= m_easingTimer; }

	void Start();

};


class GameStartTimerUI {

private:

	/*===== メンバ変数 =====*/

	std::array<std::shared_ptr<GameStartTimer>, 4> m_timer;	// タイマークラス。

	Vec2<float> m_offsetPos;

	int m_countTimer;
	const int COUNT_TIMER = 60;

	bool m_isActive;
	bool m_isStartGo;	// Goのテクスチャをスタートさせたか。

public:

	/*===== メンバ関数 =====*/

	GameStartTimerUI();
	void Init();
	void Update(const Vec2<float>& OffsetPos, const float& AddEasingTimer);
	void Draw();

	// 出現している状態か
	bool GetIsActive() { return m_isActive; }

	void Start();

};