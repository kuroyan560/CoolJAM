#include "BonusEffect.h"
#include"../engine/DrawFunc2D.h"

BonusEffect::BonusEffect() :LIMIT_MIN_LINE(-1000.0f), LIMIT_MAX_LINE(1580.0f), APPEAR_TIME(120)
{
	m_feverEffectTexBuffer = D3D12App::Instance()->GenerateTextureBuffer("resource/user/BonusStage/bonus_stage_gradation.png");
	m_bonusStageTexBuffer = D3D12App::Instance()->GenerateTextureBuffer("resource/user/BonusStage/bonus_stage.png");
	m_startFlag = false;
	m_stopFlag = false;

	m_size = { 1.0f,1.0f };
	m_pos = { 200.0f,LIMIT_MIN_LINE };
	m_vel.y = (LIMIT_MAX_LINE + abs(LIMIT_MIN_LINE)) / static_cast<float>(APPEAR_TIME);

	appearDistance = -1200.0f;

	appearRateData.Init({ 0.0f, LIMIT_MAX_LINE });
	disappearRateData.Init({ 0.0f, LIMIT_MAX_LINE + appearDistance });
	m_bigFontTimer = 0;
	m_fontSize = { 1.0f,1.0f };
}

void BonusEffect::Init()
{
}

void BonusEffect::Update()
{
	if (!m_startFlag)
	{
		return;
	}

	if (m_startFlag && !m_stopFlag)
	{
		appearRateData.Ease(appearDistance, 30, Out);
		fontPos = appearRateData.m_pos;
	}
	if (m_stopFlag)
	{
		disappearRateData.Ease(-1000.0f, 30, In);
		fontPos = disappearRateData.m_pos;
	}
	fontPos.x = 640.0f;


	if (m_startFlag || m_stopFlag)
	{
		++m_bigFontTimer;
	}
	if (30 <= m_bigFontTimer)
	{
		m_larpFontSize = { 2.0f,2.0f };
		m_bigFontTimer = 0;
	}
	else
	{
		m_larpFontSize = { 1.0f,1.0f };
	}
	m_fontSize = KuroMath::Lerp(m_fontSize, m_larpFontSize, 0.2f);



	m_pos += m_vel;
	bool restartFlag = LIMIT_MAX_LINE <= m_pos.y;
	if (restartFlag)
	{
		m_pos.y = LIMIT_MIN_LINE;
	}
	if (restartFlag && m_stopFlag && 1.0f <= disappearRateData.m_rate)
	{
		appearRateData.Init({ 0.0f, LIMIT_MAX_LINE });
		disappearRateData.Init({ 0.0f, LIMIT_MAX_LINE + appearDistance });
		m_bigFontTimer = 0;
		m_startFlag = false;
		m_stopFlag = false;
	}
}

void BonusEffect::Draw()
{
	if (!m_startFlag)
	{
		return;
	}

	for (int i = 0; i < 5; ++i)
	{
		Vec2<float>adjPos = Vec2<float>(512.0f * static_cast<float>(i), 0.0f);

		DrawFunc2D::DrawRotaGraph2D(m_pos + adjPos, m_size, 0.0f, m_feverEffectTexBuffer);
	}

	DrawFunc2D::DrawRotaGraph2D(fontPos, m_fontSize, 0.0f, m_bonusStageTexBuffer);

}

void BonusEffect::Start()
{
	m_startFlag = true;
}

void BonusEffect::Stop()
{
	m_stopFlag = true;
}

bool BonusEffect::IsStart()
{
	return m_startFlag;
}
