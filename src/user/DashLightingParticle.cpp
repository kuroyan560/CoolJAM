#include "DashLightingParticle.h"
#include"DrawFunc_Append.h"
#include"../engine/DrawFunc3D.h"
#include"../engine/Model.h"

DashLightingParticle::DashLightingParticle(std::shared_ptr<ModelObject> MODEL, std::array<std::shared_ptr<TextureBuffer>, 3>ELEC_TEXTURE_DATA) :initFlag(false)
{
	m_model = MODEL->m_model;
	elecTextureBuffer = ELEC_TEXTURE_DATA;
	//m_model->m_meshes[0].material->texBuff[0] = elecTextureBuffer[0];
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
	m_transform.SetScale({ 10.0f,10.0f,10.0f });
	//m_transform.SetRotate(Vec3<Angle>(KuroFunc::GetRand(0, 360), KuroFunc::GetRand(0, 360), KuroFunc::GetRand(0, 360)));
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
		DrawFunc_Append::DrawModel(m_model, m_transform, RenderTargetSwitch(alpha, 0.0f, 0), true, false, nullptr, AlphaBlendMode_Trans);
	}
	//Transform transform;
	//transform.SetPos({ 0.0f,5.0f,0.0f });
	//transform.SetScale({ 10.0f,10.0f,10.0f });
	//DrawFunc_Append::DrawModel(m_model, transform, RenderTargetSwitch(1.0f, 0.0f, 0.0f), true, false, nullptr, AlphaBlendMode_Trans);
}
