#include "DashLightingParticle.h"
#include"../engine/DrawFunc3D.h"

DashLightingParticle::DashLightingParticle() :initFlag(false)
{
}

void DashLightingParticle::Init(const Vec3<float> &POS)
{
	m_pos = POS;
	angle = KuroFunc::GetRand(0, 360);
	angleVel = KuroFunc::GetRand(-1.0f, 1.0f);
	alpha = 255;
	initFlag = true;

	const float R = 2.0f;
	float cosfNum = cosf(Angle::ConvertToRadian(angle)) * R;
	float sinfNum = sinf(Angle::ConvertToRadian(angle)) * R;
	float h = KuroFunc::GetRand(0.0f, R);
	m_startPos = m_pos + Vec3<float>(cosfNum, h, sinfNum);
	m_endPos = m_pos + -Vec3<float>(cosfNum, h, sinfNum);
}

void DashLightingParticle::Update()
{
	if (initFlag)
	{
		alpha -= 255.0f / 30.0f;
		if (alpha <= 0)
		{
			initFlag = false;
		}
	}
}

void DashLightingParticle::Draw(Camera &CAMERA)
{
	if (initFlag)
	{
		DrawFunc3D::DrawLine(CAMERA, m_startPos, m_endPos, Color(255, 255, 0, alpha), 0.5f);
	}
}
