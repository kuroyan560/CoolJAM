#pragma once
#include "D3D12App.h"
#include "D3D12Data.h"
#include "Vec.h"
#include "WinApp.h"
#include <string>
#include <memory>

class TutorialText {

private:

	/*===== メンバ変数 =====*/

	std::shared_ptr<TextureBuffer> m_texture;		// テクスチャ

	Vec2<float> m_pos;
	int m_appearNextDelay;		// 出現が完了してから次に以降できるようになるまでの遅延。
	const int APPEAR_NEXT_DELAY = 30;
	bool m_isAppear;			// 出現中か
	bool m_isExit;				// 退出中か
	bool m_isActive;			// 有効化されているか
	bool m_isFinish;			// すべての処理が終わったか。
	float m_textureEasingTimer;	// イージングに使用するタイマー
	const float ADD_TEXTURE_EASING_TIMER = 0.05f;

	const Vec2<float> START_POS = Vec2<float>(WinApp::Instance()->GetExpandWinCenter().x, WinApp::Instance()->GetExpandWinSize().y + WinApp::Instance()->GetExpandWinCenter().y);
	const Vec2<float> MIDDLE_POS = WinApp::Instance()->GetExpandWinCenter();
	const Vec2<float> END_POS = Vec2<float>(WinApp::Instance()->GetExpandWinCenter().x, -WinApp::Instance()->GetExpandWinCenter().y);


public:

	/*===== メンバ関数 =====*/

	TutorialText(std::string File);
	void Init();
	void Update();
	void Draw();

	bool GetIsActive() { return m_isActive; }
	bool GetisFinish() { return m_isFinish; }
	bool GetIsExit() { return m_isExit; }
	bool GetCanExit() { return APPEAR_NEXT_DELAY <= m_appearNextDelay; }
	float GetEasingTimer() { return m_textureEasingTimer; }

	void Activate();	// 有効化して真ん中に動かす。
	void Exit();		// 真ん中から上に移動させる。移動し終わったら有効化フラグが消える。

};