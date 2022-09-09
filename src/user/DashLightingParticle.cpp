#include "DashLightingParticle.h"
#include"../engine/DrawFunc3D.h"

DashLightingParticle::DashLightingParticle() :initFlag(false)
{
}

void DashLightingParticle::Init(const Vec3<float> &POS)
{
	m_pos = POS;
	angle = 0.0f;
	angleVel = KuroFunc::GetRand(-1.0f, 1.0f);
	alpha = 255;
	initFlag = true;
}

void DashLightingParticle::Update()
{
	if (initFlag)
	{
		m_pos.y += 1.0f;
		const float R = 1.5f;
		m_startPos = m_pos + Vec3<float>(cosf(Angle::ConvertToRadian(angle) * R), 0.0f, sinf(Angle::ConvertToRadian(angle) * R));
		m_endPos = m_pos + -Vec3<float>(cosf(Angle::ConvertToRadian(angle) * R), 0.0f, sinf(Angle::ConvertToRadian(angle) * R));
		angle += angleVel;

		alpha -= 5;
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
		DrawFunc3D::DrawLine(CAMERA, m_startPos, m_endPos, Color(255, 255, 0, alpha), 1.3f);
	}
}
