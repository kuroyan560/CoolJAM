#pragma once
#include "Singleton.h"

class SlowMgr : public Singleton<SlowMgr> {

public:

	/*===== メンバ変数 =====*/

	float m_slow;		// スロー
	float m_baseSlow;	// 補間先のスロー


public:

	/*===== メンバ関数 =====*/

	void Init() {

		/*===== 初期化処理 =====*/

		m_slow = true;
		m_baseSlow = true;

	}

	// 更新処理
	void Update() {

		/*===== 更新処理 ======*/

		if (m_baseSlow < 0.0f) m_baseSlow = 0.0f;
		if (1.0f <= m_baseSlow) m_baseSlow = 1.0f;

		m_slow += (m_baseSlow - m_slow) / 10.0f;

	}

};