#include "PlayerModelOutline.h"
#include"DrawFunc3D.h"
#include"../engine/Common/KuroMath.h"
#include"../user/KazDrawFunc.h"

PlayerModelOutline::PlayerModelOutline()
{
}

void PlayerModelOutline::Init(Vec3<float> *POS, DirectX::XMMATRIX *ROTATION,float PLAYER_SCALE, float EXPAND_SCALE, std::shared_ptr<Model> MODEL)
{
	m_pos = POS;
	m_rotation = ROTATION;
	m_baseScale = PLAYER_SCALE;
	m_expandMaxScale = EXPAND_SCALE;
	m_model = MODEL;
	m_powerUpFlag = false;
}

void PlayerModelOutline::Update()
{
	const float MAX_SCALE = m_baseScale;
	const float EXPAND_MAX_SCALE = m_expandMaxScale;

	//ã≠âªââèoíÜ
	if (m_enoughPowerFlag)
	{
		if (m_reversRateFlag)
		{
			Rate(&m_expandVertexRate, 0.3f, 1.0f);
			m_scale = (MAX_SCALE + EXPAND_MAX_SCALE) + -KuroMath::Ease(Out, Cubic, m_expandVertexRate, 0.0f, 1.0f) * EXPAND_MAX_SCALE;
		}
		else
		{
			Rate(&m_expandVertexRate, 0.3f, 1.0f);
			m_scale = MAX_SCALE + KuroMath::Ease(Out, Cubic, m_expandVertexRate, 0.0f, 1.0f) * EXPAND_MAX_SCALE;
		}

		if (1.0f <= m_expandVertexRate)
		{
			m_reversRateFlag = !m_reversRateFlag;
		}

		if (m_reversRateFlag != m_prevReversRateFlag)
		{
			m_expandVertexRate = 0.0f;
		}
		m_prevReversRateFlag = m_reversRateFlag;
		m_color.m_a = 255;
	}
	//í èÌéû
	else if(m_powerUpFlag)
	{
		float perRate = 0.01f + 0.06f * (m_powerNum / 100.0f);
		Rate(&m_expandVertexRate, perRate, 1.0f);
		m_scale = MAX_SCALE + KuroMath::Ease(Out, Cubic, m_expandVertexRate, 0.0f, 1.0f) * EXPAND_MAX_SCALE;
		m_color.m_a = static_cast<int>(255.0f * (1.0f - m_expandVertexRate));
		if (1.0f <= m_expandVertexRate)
		{
			m_expandVertexRate = 0.0f;
		}
	}
	//âΩÇ‡ÇµÇ»Ç¢
	else
	{
		m_color.m_a = 255;
		m_scale = 0.0f;
	}
	m_transform.SetPos(*m_pos);
	m_transform.SetScale({  m_scale,m_scale,m_scale });
	m_transform.SetRotate(*m_rotation);

	m_enoughPowerFlag = false;
}

void PlayerModelOutline::Draw(Camera &CAMERA)
{
	return;
	KazDrawFunc::DrawNonShadingModel(m_model, m_transform, m_color, CAMERA);
}

void PlayerModelOutline::EnoughPowerEffect()
{
	m_enoughPowerFlag = true;
}

void PlayerModelOutline::PowerUpEffect(int SPEED)
{
	m_powerUpFlag = true;
	m_powerNum = static_cast<float>(SPEED);
}
