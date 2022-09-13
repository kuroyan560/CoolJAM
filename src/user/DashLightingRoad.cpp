#include "DashLightingRoad.h"
#include"../engine/DrawFunc3D.h"
#include"../user/ColorPalette.h"
#include"../user/DrawFunc_Append.h"
#include"../engine/Common/KuroMath.h"

DashLightingRoad::DashLightingRoad(int ROAD_INDEX) :m_color(ColorPalette::S_GREEN_COLOR), m_initFlag(false), m_indexNum(ROAD_INDEX)
{
}

void DashLightingRoad::Init(const Vec3<float> &POS)
{
	m_startPos = POS;
	m_baseStartPos = POS;
	m_lenghtRate = 0.0f;
	m_initFlag = true;
	m_disappearFlag = false;
	m_startTimer = 0;
	m_timer = 0;
}

void DashLightingRoad::Update(const Vec3<float> &POS, int NOW_INDEX, bool DISAPPEAR_FLAG)
{
	if (!m_initFlag)
	{
		return;
	}

	if (m_indexNum == NOW_INDEX)
	{
		m_endPos = POS;
		m_distance = m_endPos.Distance(m_startPos);
	}




	++m_startTimer;

	int appearMaxTimer = 30;
	if (!DISAPPEAR_FLAG && 10 <= m_startTimer)
	{
		m_disappearFlag = true;
		m_distance = m_endPos.Distance(m_startPos);
	}
	//if (m_disappearFlag)
	{
		++m_timer;
	}

	if (appearMaxTimer <= m_timer || m_indexNum != NOW_INDEX)
	{
		float timer = 20.0f;
		if (m_distance <= 20.0f)
		{
			timer = 10.0f;
		}

		if (m_lenghtRate < 1.0f)
		{
			m_lenghtRate += 1.0f / timer;
		}
		if (1.0f <= m_lenghtRate)
		{
			m_lenghtRate = 1.0f;
		}
	}
	Vec3<float>distance = m_endPos - m_startPos;
	float easeRate = KuroMath::Ease(Out, Cubic, (1.0f - m_lenghtRate), 0.0f, 1.0f);
	m_startPos = m_endPos + -distance * easeRate;


	if (m_distance <= 0.1f && 10 <= m_timer)
	{
		m_initFlag = false;
	}

}

void DashLightingRoad::Draw(Camera &Cam)
{
	const Vec3<float>OFFSET = { 0,0.4f,0.0f };
	if (m_initFlag)
	{
		DrawFunc_Append::DrawLine(m_startPos + OFFSET,
			m_endPos + OFFSET,
			m_color, 0.3f, RenderTargetSwitch(0.5f, 0.5f, 1.0f), AlphaBlendMode_Add);
	}
}