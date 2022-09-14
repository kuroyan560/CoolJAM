#pragma once
#include "Vec.h"
#include <memory>
#include <array>

class TextureBuffer;

class FeverGauge {

private:

	/*====== �����o�ϐ� =====*/

	std::shared_ptr<TextureBuffer> m_gaugeTexture;
	std::array<std::shared_ptr<TextureBuffer>, 6> m_feverText;
	float m_baseRate;
	float m_nowRate;
	const float GAUGE_LENGTH = 300.0f;
	bool m_isActive;

	std::array<float, 6> m_sinWave;	// �T�C���g�Ɏg�p����
	const float ADD_SIN_WAVE = 0.2f;
	const float SIN_WAVE_LENGTH = 30.0f;
	float m_alpha;					// �A���t�@�l
	float m_lissajousTimer;			// ���T�[�W���Ȑ��p�^�C�}�[


public:

	/*===== �����o�֐� =====*/

	FeverGauge();
	void Init();
	void Update(const bool& IsActive, const float& Rate);
	void Draw();

};