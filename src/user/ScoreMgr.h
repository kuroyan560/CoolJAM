#pragma once
#include "Singleton.h"
#include "Vec.h"
#include "WinApp.h"
#include "D3D12App.h"
#include <array>
#include <memory>

class ScoreMgr : public Singleton<ScoreMgr> {

private:

	/*===== メンバ変数 =====*/

	Vec2<float> m_pos;

	float m_easingTimer;	// イージングのタイマー

	bool m_isAppear;		// 出現中か。
	bool m_isExit;			// 消滅中か。
	bool m_isCenter;
	bool m_isActive;

	int m_prevScore;		// 1F前のスコア 演出用
	int m_score;			// 現在のスコア
	int m_addScoreRegister;	// スコアに加算する量。スコアを加算する際は一度この値に入れる。スコアががらがら増える演出のため。

	const float FONT_OFFSET = 64.0f;

	float m_scoreScale;
	const float DEF_SCALE = 0.5f;

	std::shared_ptr<TextureBuffer> m_ScoreTexture;

	const Vec2<float> MIDDLE_POS = WinApp::Instance()->GetExpandWinCenter() + Vec2<float>(0, 300.0f);
	const Vec2<float> CENTER_POS = WinApp::Instance()->GetExpandWinCenter();
	const Vec2<float> APPEAR_POS = Vec2<float>(WinApp::Instance()->GetExpandWinCenter().x, WinApp::Instance()->GetExpandWinSize().y + WinApp::Instance()->GetExpandWinCenter().y);
	const Vec2<float> EXIT_POS = Vec2<float>(WinApp::Instance()->GetExpandWinCenter().x, -WinApp::Instance()->GetExpandWinCenter().y);


public:

	/*===== メンバ関数 =====*/

	void Init();
	void Update(const Vec2<float>& Offset, const Vec2<float>& CenterOffset, const float& AddEasingTimer);
	void Draw();

	// 出現している状態か
	bool GetIsActive() { return m_isActive; }

	void Appear();
	void Exit();
	void Center();	// Appearで所定の位置に移動させた後に読んでください。

	void AddScore(const int& Score);	// スコアを加算する。
	void GameFinish();					// 保存してあるスコアを一気に代入する。

};