#include "DashLightingParticle.h"
#include"DrawFunc_Append.h"
#include"../engine/DrawFunc3D.h"

DashLightingParticle::DashLightingParticle(std::shared_ptr<ModelObject> MODEL) :initFlag(false)
{
	m_model = MODEL->m_model;
}

void DashLightingParticle::Init(const Vec3<float> &POS)
{
	m_pos = POS;
	angle = KuroFunc::GetRand(0, 360);
	angleVel = KuroFunc::GetRand(-1.0f, 1.0f);
	alpha = 1;
	initFlag = true;

	const float R = 2.0f;
	float cosfNum = cosf(Angle::ConvertToRadian(angle)) * R;
	float sinfNum = sinf(Angle::ConvertToRadian(angle)) * R;
	float h = KuroFunc::GetRand(0.0f, R);
	m_startPos = m_pos + Vec3<float>(cosfNum, h, sinfNum);
	m_endPos = m_pos + -Vec3<float>(cosfNum, h, sinfNum);

	m_transform.SetPos(m_startPos);
	m_transform.SetScale({ 1.0f,1.0f,1.5f });
	m_transform.SetRotate(Vec3<Angle>(KuroFunc::GetRand(0, 360), KuroFunc::GetRand(0, 360), KuroFunc::GetRand(0, 360)));
}

void DashLightingParticle::Update()
{
	if (initFlag)
	{
		alpha -= 1.0f / 30.0f;
		if (alpha <= 0.0f)
		{
			initFlag = false;
		}
	}
}

#include"ColorPalette.h"
void DashLightingParticle::Draw(Camera &CAMERA)
{
	if (initFlag)
	{
		DrawFunc_Append::DrawModel(m_model, m_transform, RenderTargetSwitch(alpha, alpha, 0), true);
	}
}
