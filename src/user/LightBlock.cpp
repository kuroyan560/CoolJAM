#include"LightBlock.h"
#include"DrawFunc_Append.h"

LightBlock::LightBlock(std::shared_ptr<Model> MODEL) :M_MAX_HIGHT(1000.0f)
{
	m_model = MODEL;
	m_pos.y = M_MAX_HIGHT;
	m_timer = KuroFunc::GetRand(60.0f, 120.0f);
	m_tranform.SetScale({ 2.0f,6.0f,2.0f });
	m_finishFlag = false;
	m_stopFlag = false;
}

void LightBlock::Init(const Vec3<float> &POS)
{
	m_pos = POS;
	m_pos.y = M_MAX_HIGHT;
	m_vel.y = -M_MAX_HIGHT / static_cast<float>(m_timer);
	m_finishFlag = false;
	m_stopFlag = false;
}

void LightBlock::Update()
{
	if (!m_finishFlag)
	{
		m_pos += m_vel;
	}

	if (m_pos.y <= -M_MAX_HIGHT)
	{
		m_pos.y = M_MAX_HIGHT;

		if (m_stopFlag)
		{
			m_finishFlag = true;
		}
	}
	m_tranform.SetPos(m_pos);
}

void LightBlock::Draw(Camera &CAMERA)
{
	if (!m_finishFlag)
	{
		DrawFunc_Append::DrawModel(m_model, m_tranform, RenderTargetSwitch(0.0f, 1.0f, 1.0f));
	}
}

void LightBlock::Stop()
{
	m_stopFlag = true;
}
