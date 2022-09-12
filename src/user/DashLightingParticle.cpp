#include "DashLightingParticle.h"
#include"DrawFunc_Append.h"

DashLightingParticle::DashLightingParticle() :initFlag(false)
{
	firstDrawFlag = false;
	//m_model = std::make_shared<ModelObject>("resource/user/Particle/", "plane.glb");
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

	//m_model->m_transform.SetPos(m_startPos);
	//m_model->m_transform.SetScale({ 0.2f,1.0f,1.0f });
	//m_model->m_transform.SetRotate(Vec3<float>(0, 0, 0));
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

#include"ColorPalette.h"
void DashLightingParticle::Draw(Camera &CAMERA)
{
	//if (initFlag || !firstDrawFlag)
	if (initFlag)
	{
		float a = alpha / 255.0f;
		Color col = ColorPalette::S_GREEN_COLOR;

		DrawFunc_Append::DrawLine(m_startPos, m_endPos, col, 0.5f,
			RenderTargetSwitch(a, a, 0.0f));
		firstDrawFlag = true;
	}
}
