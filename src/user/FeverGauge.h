#pragma once
#include "Vec.h"
#include <memory>

class TextureBuffer;

class FeverGauge {

private:

	/*====== �����o�ϐ� =====*/

	std::shared_ptr<TextureBuffer> m_gaugeTexture;
	float m_baseRate;
	float m_nowRate;
	const float GAUGE_LENGTH = 300.0f;
	bool m_isActive;

public:

	/*===== �����o�֐� =====*/

	FeverGauge();
	void Init();
	void Update(const bool& IsActive, const float& Rate);
	void Draw();

};