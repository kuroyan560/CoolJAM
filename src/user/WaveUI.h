#pragma once
#include "Vec.h"
#include "D3D12App.h"
#include "D3D12Data.h"
#include "WinApp.h"
#include <string>
#include <memory>

class WaveUI {

private:

	/*===== メンバ変数 =====*/

	std::shared_ptr<TextureBuffer> m_waveTexture;	// 「WAVE」のテクスチャ

	Vec2<float> m_wavePos;

	float m_easingTimer;	// イージングのタイマー
	const float ADD_EASING_TIMER = 0.05f;

	bool m_isAppear;		// 出現中か。
	bool m_isExit;			// 消滅中か。
	bool m_isActive;

	const float DEF_SCALE = 1.0f;
	const Vec2<float> MIDDLE_POS = WinApp::Instance()->GetExpandWinCenter() + Vec2<float>(0, 1.0f);
	const Vec2<float> APPEAR_POS = Vec2<float>(WinApp::Instance()->GetExpandWinCenter().x, WinApp::Instance()->GetExpandWinSize().y + WinApp::Instance()->GetExpandWinCenter().y) + Vec2<float>(0, 1.0f);
	const Vec2<float> EXIT_POS = Vec2<float>(WinApp::Instance()->GetExpandWinCenter().x, -WinApp::Instance()->GetExpandWinCenter().y) + Vec2<float>(0, 1.0f);


public:

	/*===== メンバ関数 =====*/

	WaveUI(std::string File);
	void Init();
	void Update();
	void Draw();

	// 出現している状態か
	bool GetIsActive() { return m_isActive; }

	void Appear();
	void Exit();

};