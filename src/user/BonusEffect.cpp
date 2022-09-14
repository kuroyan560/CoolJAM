#include "BonusEffect.h"
#include"../engine/DrawFunc2D.h"

BonusEffect::BonusEffect() :LIMIT_MIN_LINE(-1000.0f), LIMIT_MAX_LINE(1580.0f), APPEAR_TIME(60)
{
	m_feverEffectTexBuffer = D3D12App::Instance()->GenerateTextureBuffer("resource/user/BonusStage/BonusEffect.png");
	m_startFlag = false;
	m_stopFlag = false;

	m_size = { 100.0f,20.0f };
	m_pos = { 500.0f,LIMIT_MIN_LINE };
	m_vel.y = (LIMIT_MAX_LINE + abs(LIMIT_MIN_LINE)) / static_cast<float>(APPEAR_TIME);

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


	m_pos += m_vel;
	bool restartFlag = LIMIT_MAX_LINE <= m_pos.y;
	if (restartFlag)
	{
		m_pos.y = LIMIT_MIN_LINE;
	}
	if (restartFlag && m_stopFlag)
	{
		m_startFlag = false;
	}
}

void BonusEffect::Draw()
{
	if (!m_startFlag)
	{
		return;
	}
	DrawFunc2D::DrawRotaGraph2D(m_pos, m_size, 0.0f, m_feverEffectTexBuffer);
}

void BonusEffect::Start()
{
	m_startFlag = true;
}

void BonusEffect::Stop()
{
	m_stopFlag = true;
}
