#include "DashLightingRoad.h"
#include"../engine/DrawFunc3D.h"
#include"../user/ColorPalette.h"
#include"../user/DrawFunc_Append.h"

int DashLightingRoad::INDEX = 0;

DashLightingRoad::DashLightingRoad():color(ColorPalette::S_GREEN_COLOR), initFlag(false)
{
	indexNum = INDEX;
	++INDEX;
	firstDrawFlag = false;
}

void DashLightingRoad::Init(const Vec3<float> &POS)
{
	startPos = POS;
	color.m_a = 255;
	initFlag = true;
}

void DashLightingRoad::Update(const Vec3<float> &POS, int NOW_INDEX)
{
	if (initFlag)
	{
		
		if (indexNum != NOW_INDEX)
		{
			color.m_a -= (1.0f / 30.0f);
		}
		else
		{
			endPos = POS;
		}

		if (color.m_a <= 0)
		{
			initFlag = false;
			color.m_a = 0;
		}
	}
}

void DashLightingRoad::Draw(Camera &Cam)
{
	if (initFlag || !firstDrawFlag)
	{
		DrawFunc_Append::DrawLine(startPos, endPos, color, 1.0f, RenderTargetSwitch(0.0f, 1.0f, 1.0f), AlphaBlendMode_Add);
		firstDrawFlag = true;
	}
}