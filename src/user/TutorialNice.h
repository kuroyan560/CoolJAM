#pragma once
#include "Vec.h"
#include "D3D12App.h"
#include "D3D12Data.h"
#include "WinApp.h"
#include <string>
#include <memory>

class TutorialNice {

private:

	/*===== メンバ変数 =====*/

	std::shared_ptr<TextureBuffer> m_niceTexture;	// NICEのテクスチャ

	float m_easingTimer;// イージングのタイマー
	const float ADD_EASING_TIMER = 0.05f;
	float m_scale;		// 拡大率
	float m_alpha;		// 不透明度

	int m_exitTimer;	// 消えるまでのタイマー
	const int EXIT_TIMER = 60;

	bool m_isAppear;	// 出現中か。
	bool m_isExit;		// 消滅中か。
	bool m_isActive;

	const float DEF_SCALE = 1.3f;
	const float EXP_SCALE = 4.0f;
	const Vec2<float> DRAW_POS = WinApp::Instance()->GetExpandWinCenter() + Vec2<float>(0.0f, 200.0f);


public:

	/*===== メンバ関数 =====*/

	TutorialNice(std::string File);
	void Init();
	void Update();
	void Draw();

	// 出現している状態か
	bool GetIsActive() { return m_isActive; }
	bool GetIsLowAlpha() { return m_alpha < 0.2f; }
	bool GetIsExit() { return m_isExit; }

	void Appear();

};