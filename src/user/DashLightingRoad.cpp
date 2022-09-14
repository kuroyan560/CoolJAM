#include "DashLightingRoad.h"
#include"../engine/DrawFunc3D.h"
#include"../user/ColorPalette.h"
#include"../user/DrawFunc_Append.h"

DashLightingRoad::DashLightingRoad(int ROAD_INDEX) :m_color(ColorPalette::S_GREEN_COLOR), m_initFlag(false), m_indexNum(ROAD_INDEX)
{
}

void DashLightingRoad::Init(const Vec3<float> &POS)
{
	m_startPos = POS;
	m_color.m_a = 1.0f;
	m_initFlag = true;
}

void DashLightingRoad::Update(const Vec3<float> &POS, int NOW_INDEX)
{
	if (m_initFlag)
	{
		if (m_indexNum != NOW_INDEX)
		{
			m_color.m_a -= (1.0f / 30.0f);
		}
		else
		{
			m_endPos = POS;
		}

		if (m_color.m_a <= 0)
		{
			m_initFlag = false;
			m_color.m_a = 0;
		}
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