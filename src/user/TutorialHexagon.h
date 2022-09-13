#pragma once
#include "Vec.h"
#include "D3D12App.h"
#include "D3D12Data.h"
#include "WinApp.h"
#include <string>
#include <memory>

class TutorialHexagon {

private:

	/*===== メンバ変数 =====*/

	std::shared_ptr<TextureBuffer> m_hexagonTex;	// 六角形のテクスチャ

	float m_easingTimer;// イージングのタイマー
	const float ADD_EASING_TIMER = 0.05f;
	float m_rotate;		// 回転
	float m_scale;		// 拡大率
	float m_alpha;		// 不透明度

	bool m_isExp;		// 拡大フラグ(一回のイージングのうち、拡大している状態かを表す。)
	bool m_isAppear;	// 出現中か。
	bool m_isNormal;	// 通常状態か。
	bool m_isExit;		// 消滅中か。

	const float DEF_SCALE = 1.0f;
	const float EXP_SCALE = 1.8f;
	const float EASE_ROTATE = DirectX::XM_2PI * 1.0f;
	const Vec2<float> DRAW_POS = WinApp::Instance()->GetExpandWinCenter() + Vec2<float>(0.0f, 200.0f);


public:

	/*===== メンバ関数 =====*/

	TutorialHexagon(std::string File);
	void Init();
	void Update();
	void Draw();

	// 出現している状態か
	bool GetIsAlive() { return m_isAppear || m_isNormal; }

	void Appear();
	void Exit();

};