#pragma once
#include "Singleton.h"

class SlowMgr : public Singleton<SlowMgr> {

public:

	/*===== �����o�ϐ� =====*/

	float m_slow;		// �X���[
	float m_baseSlow;	// ��Ԑ�̃X���[


public:

	/*===== �����o�֐� =====*/

	void Init() {

		/*===== ���������� =====*/

		m_slow = 1.0f;
		m_baseSlow = 1.0f;

	}

	// �X�V����
	void Update() {

		/*===== �X�V���� ======*/

		if (m_baseSlow < 0.0f) m_baseSlow = 0.0f;
		if (1.0f <= m_baseSlow) m_baseSlow = 1.0f;

		m_slow += (m_baseSlow - m_slow) / 10.0f;

	}

};