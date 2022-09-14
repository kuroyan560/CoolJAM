#pragma once
#include"../engine/Common/Vec.h"
#include"../engine/D3D12App.h"

class BonusEffect
{
public:
	BonusEffect();
	void Init();
	void Update();
	void Draw();

	void Start();
	void Stop();

	bool IsStart();
private:
	Vec2<float>m_pos, m_size, m_vel;
	std::shared_ptr<TextureBuffer>m_feverEffectTexBuffer;
	std::shared_ptr<TextureBuffer>m_bonusStageTexBuffer;

	bool m_startFlag;
	bool m_stopFlag;

	const float LIMIT_MIN_LINE, LIMIT_MAX_LINE;
	const int APPEAR_TIME;


	struct EaseData
	{
		float m_rate;
		Vec2<float> m_basePos;
		Vec2<float> m_pos;

		void Init(const Vec2<float> &BASE_POS)
		{
			m_rate = 0.0f;
			m_basePos = BASE_POS;
			m_pos = {};
		};

		void Ease(float DISTANCE, int TIMER, EASE_CHANGE_TYPE EASE)
		{
			if (m_rate < 1.0f)
			{
				m_rate += 1.0f / static_cast<float>(TIMER);
			}
			if (1.0f <= m_rate)
			{
				m_rate = 1.0f;
			}
			m_pos = m_basePos + KuroMath::Ease(EASE, Circ, m_rate, Vec2<float>(0.0f, 0.0f), Vec2<float>(1.0f, 1.0f)) * DISTANCE;
		};
	};

	EaseData appearRateData;
	EaseData disappearRateData;
	Vec2<float>fontPos, m_fontSize, m_larpFontSize;
	int m_bigFontTimer;
	float appearDistance;
};
