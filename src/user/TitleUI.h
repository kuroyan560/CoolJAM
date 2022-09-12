#pragma once
#include "Vec.h"
#include "D3D12Data.h"
#include <memory>
#include <array>
#include <string>

class TitleUI {

private:

	/*===== メンバ変数 =====*/

	std::shared_ptr<TextureBuffer> m_texBuffer;	// テクスチャ
	float m_angle;								// 現在のアングル
	float m_easingTimer;						// イージングに使用するタイマー
	float m_alpha;								// アルファ値。
	const float SIDE_ALPHA = 0.2f;				// 上、下にいるときのアルファ値
	const float ADD_EASING_TIMER = 0.1f;		// イージングの加算量。

public:

	enum class STATUS {

		UP,
		MIDDLE,
		DOWN,

	};

	enum class UI_STATUS {

		TUTORIAL,
		GAME,
		EXIT,

	};

private:

	STATUS m_status;
	STATUS m_nextStatus;
	STATUS m_defStatus;
	UI_STATUS m_UIStatus;

	// 回転量
	const float ROTATE_REVOLVER_UI = DirectX::XM_PIDIV4;
	// 回転量の初期値。
	const std::array<float, 3> DEF_ROTATE = { DirectX::XM_2PI - ROTATE_REVOLVER_UI, 0.0f , ROTATE_REVOLVER_UI , };

public:

	/*===== メンバ関数 =====*/

	TitleUI(std::string TexPath, STATUS Status, UI_STATUS UIStatus);
	void Init();
	void Update();
	void Draw(const Vec2<float>& RevolverPos);

	void TransUp();
	void TransDown();
	bool GetCanTrans() { return 1.0f <= m_easingTimer; };

private:

	void ChangeUpdateNowUp();
	void ChangeUpdateNowMiddle();
	void ChangeUpdateNowDown();
	void Trans(STATUS NextStatus);

};