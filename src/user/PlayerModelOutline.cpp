#include "PlayerModelOutline.h"
#include"../engine/DrawFuncBillBoard.h"
#include"../engine/Common/KuroMath.h"
#include"../user/KazDrawFunc.h"
#include"ColorPalette.h"
#include"../engine/Importer.h"

PlayerModelOutline::PlayerModelOutline()
{
	m_color = ColorPalette::S_GREEN_COLOR;
	waveTex = Importer::Instance()->LoadModel("resource/user/Particle/", "powerWave.glb");
}

void PlayerModelOutline::Init(Vec3<float> *POS, DirectX::XMMATRIX *ROTATION, float PLAYER_SCALE, float EXPAND_SCALE, std::shared_ptr<Model> MODEL)
{
	m_pos = POS;
	m_rotation = ROTATION;
	m_baseScale = PLAYER_SCALE;
	m_expandMaxScale = EXPAND_SCALE;
	m_model = MODEL;
	m_powerUpFlag = false;

	m_enoughPowerRateData.Init();
	m_powerUpRateData.Init();
	m_waveTextureSize = { 0.0f,0.0f };
}

void PlayerModelOutline::Update()
{
	const float MAX_SCALE = m_baseScale;
	const float EXPAND_MAX_SCALE = m_expandMaxScale;

	//ã≠âªââèoíÜ
	if (m_powerUpFlag)
	{
		float expandScale = (1.0f - m_powerUpRateData.m_countReversNum / m_powerUpRateData.m_countReversMaxNum);

		if (m_powerUpRateData.m_reversRateFlag)
		{
			Rate(&m_powerUpRateData.m_expandVertexRate, 0.3f, 1.0f);
			m_scale = (MAX_SCALE + EXPAND_MAX_SCALE * expandScale) + -KuroMath::Ease(Out, Cubic, m_powerUpRateData.m_expandVertexRate, 0.0f, 1.0f) * (EXPAND_MAX_SCALE * expandScale);
		}
		else
		{
			Rate(&m_powerUpRateData.m_expandVertexRate, 0.3f, 1.0f);
			m_scale = MAX_SCALE + KuroMath::Ease(Out, Cubic, m_powerUpRateData.m_expandVertexRate, 0.0f, 1.0f) * (EXPAND_MAX_SCALE * expandScale);
		}

		if (1.0f <= m_powerUpRateData.m_expandVertexRate)
		{
			m_powerUpRateData.m_reversRateFlag = !m_powerUpRateData.m_reversRateFlag;
			++m_powerUpRateData.m_countReversNum;
			m_powerUpRateData.m_expandVertexRate = 0.0f;
		}
		if (m_powerUpRateData.m_countReversMaxNum <= m_powerUpRateData.m_countReversNum)
		{
			m_powerUpRateData.m_countReversNum = m_powerUpRateData.m_countReversMaxNum;
		}
	}
	else
	{
		m_powerUpRateData.Init();
	}
	//ã≠âªäÆóπ
	if (m_enoughPowerFlag)
	{
		float expandScele = 0.0f;
		if (2 <= m_enoughPowerRateData.m_countReversNum)
		{
			expandScele = EXPAND_MAX_SCALE - 0.2f;
		}
		else
		{
			expandScele = EXPAND_MAX_SCALE * 1.1f;
		}

		if (m_enoughPowerRateData.m_reversRateFlag)
		{
			Rate(&m_enoughPowerRateData.m_expandVertexRate, 0.3f, 1.0f);
			m_scale = (MAX_SCALE + expandScele) + -KuroMath::Ease(Out, Cubic, m_enoughPowerRateData.m_expandVertexRate, 0.0f, 1.0f) * expandScele;
		}
		else
		{
			Rate(&m_enoughPowerRateData.m_expandVertexRate, 0.3f, 1.0f);
			m_scale = MAX_SCALE + KuroMath::Ease(Out, Cubic, m_enoughPowerRateData.m_expandVertexRate, 0.0f, 1.0f) * expandScele;
		}

		if (1.0f <= m_enoughPowerRateData.m_expandVertexRate)
		{
			m_enoughPowerRateData.m_reversRateFlag = !m_enoughPowerRateData.m_reversRateFlag;
			++m_enoughPowerRateData.m_countReversNum;
			m_enoughPowerRateData.m_expandVertexRate = 0.0f;
		}

		m_color.m_a -= 1.0f / 30.0f;
		m_waveTextureSize.x += 0.1f;
		m_waveTextureSize.y += 0.1f;
		if (m_color.m_a <= 0.0f)
		{
			m_color.m_a = 0.0f;
		}
	}
	else
	{
		m_enoughPowerRateData.Init();
	}

	m_transform.SetPos(*m_pos);
	m_transform.SetScale({ m_scale,m_scale,m_scale });
	m_transform.SetRotate(*m_rotation);

	m_enoughPowerFlag = false;
	m_powerUpFlag = false;

	m_waveTransform.SetPos(*m_pos);
	m_waveTransform.SetScale({ m_waveTextureSize.x,m_waveTextureSize.y,m_waveTextureSize.x });
	m_waveTransform.SetRotate(Vec3<Angle>(0, 0, 0));
}

void PlayerModelOutline::Draw(Camera &CAMERA)
{
	KazDrawFunc::DrawNonShadingModelSignalColor(m_model, m_transform, m_color, CAMERA);
	DrawFunc_Append::DrawModel(waveTex, m_waveTransform, RenderTargetSwitch(m_color.m_a, 0.0f, 1.0f));
}

void PlayerModelOutline::EnoughPowerEffect()
{
	m_enoughPowerFlag = true;
}

void PlayerModelOutline::PowerUpEffect(int SPEED)
{
	m_powerUpFlag = true;
	m_powerUpRateData.m_powerNum = static_cast<float>(SPEED);
}
