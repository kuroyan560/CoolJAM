#include"LightBlock.h"
#include"../engine/DrawFunc3D.h"

LightBlock::LightBlock(const Vec3<float> &POS, std::shared_ptr<Model> MODEL) :M_MAX_HIGHT(1000.0f)
{
	m_model = MODEL;
	m_pos = POS;
	m_timer = KuroFunc::GetRand(60.0f, 120.0f);
	m_tranform.SetScale({ 2.0f,6.0f,2.0f });
}

void LightBlock::Init()
{
	m_pos.y = M_MAX_HIGHT;
	m_vel.y = -M_MAX_HIGHT / static_cast<float>(m_timer);
}

void LightBlock::Update()
{
	m_pos += m_vel;
	if (m_pos.y <= -M_MAX_HIGHT)
	{
		m_pos.y = M_MAX_HIGHT;
	}
	m_tranform.SetPos(m_pos);
}

void LightBlock::Draw(Camera &CAMERA)
{
	DrawFunc3D::DrawNonShadingModel(m_model, m_tranform, CAMERA);
}