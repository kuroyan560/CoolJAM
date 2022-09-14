#pragma once
#include "Singleton.h"
#include "Vec.h"
#include "KuroFunc.h"

class ShakeMgr : public Singleton<ShakeMgr> {

public:

	/*===== メンバ変数 =====*/

	float m_shakeAmount;		// シェイクの最大数 シェイクをだんだん小さくする用
	const float SUB_SHAKE = 1.0f;
	Vec3<float> m_shake;		// 現在のフレームのシェイク量


public:

	/*===== 初期化処理 =====*/

	void Init() {

		m_shakeAmount = 0;
		m_shake = Vec3<float>();

	}

	void Update() {

		// シェイク量をへらす。
		m_shakeAmount -= SUB_SHAKE;
		if (m_shakeAmount < 0) {

			m_shakeAmount = 0;

		}

		// 現在のシェイク量を計算する。
		m_shake = Vec3<float>(KuroFunc::GetRand(-m_shakeAmount, m_shakeAmount), KuroFunc::GetRand(-m_shakeAmount, m_shakeAmount), KuroFunc::GetRand(-m_shakeAmount, m_shakeAmount));

	}

	// シェイクをかける。
	void Shake(const float& Value) {

		if (Value < m_shakeAmount) return;

		m_shakeAmount = Value;

	}

};