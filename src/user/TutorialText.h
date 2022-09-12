#pragma once
#include "D3D12App.h"
#include <memory>

class TutorialText {

private:

	/*===== メンバ変数 =====*/

	std::shared_ptr<RenderTarget> m_floorRenderTarget;


public:

	// チュートリアルのステータス
	enum class TUTORIAL_STATUS {

		MOUSE,			// マウスの方向にいくチュートリアル
		DASH,			// ダッシュのチュートリアル。
		DAMAGE_WALL,	// ダメージ壁の説明
		ENEMY,			// 真ん中に何もしてこない敵を配置
		FEVER,			// フィーバー状態の説明

	};

	TUTORIAL_STATUS m_tutorialStatus;


public:

	/*===== メンバ関数 =====*/

	TutorialText();
	void Init();
	void Update();
	void Draw();

	void Next();

	TUTORIAL_STATUS GetNowStatus() { return m_tutorialStatus; }

};