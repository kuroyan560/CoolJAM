#include "BonusEffect.h"
#include"../engine/DrawFunc2D.h"

BonusEffect::BonusEffect()
{
	m_feverEffectTexBuffer = D3D12App::Instance()->GenerateTextureBuffer("resource/user/BonusStage/BonusEffect.png");
	m_startFlag = false;
	m_stopFlag = false;

	m_size = { 100.0f,100.0f };
	m_pos = { 0.0f,0.0f };
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

	m_pos.y += 30.0f;
	bool restartFlag = 1580.0f <= m_pos.y;
	if (restartFlag)
	{
		m_pos.y = -1000.0f;
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
