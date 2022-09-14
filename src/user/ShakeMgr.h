#pragma once
#include "Singleton.h"
#include "Vec.h"
#include "KuroFunc.h"

class ShakeMgr : public Singleton<ShakeMgr> {

public:

	/*===== �����o�ϐ� =====*/

	float m_shakeAmount;		// �V�F�C�N�̍ő吔 �V�F�C�N�����񂾂񏬂�������p
	const float SUB_SHAKE = 1.0f;
	Vec3<float> m_shake;		// ���݂̃t���[���̃V�F�C�N��


public:

	/*===== ���������� =====*/

	void Init() {

		m_shakeAmount = 0;
		m_shake = Vec3<float>();

	}

	void Update() {

		// �V�F�C�N�ʂ��ւ炷�B
		m_shakeAmount -= SUB_SHAKE;
		if (m_shakeAmount < 0) {

			m_shakeAmount = 0;

		}

		// ���݂̃V�F�C�N�ʂ��v�Z����B
		m_shake = Vec3<float>(KuroFunc::GetRand(-m_shakeAmount, m_shakeAmount), KuroFunc::GetRand(-m_shakeAmount, m_shakeAmount), KuroFunc::GetRand(-m_shakeAmount, m_shakeAmount));

	}

	// �V�F�C�N��������B
	void Shake(const float& Value) {

		if (Value < m_shakeAmount) return;

		m_shakeAmount = Value;

	}

};