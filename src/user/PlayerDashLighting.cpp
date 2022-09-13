#include "PlayerDashLighting.h"
#include"DrawFunc3D.h"
#include"Model.h"
#include"DrawFunc_Append.h"

PlayerDashLighting::PlayerDashLighting()
{
	m_model = std::make_shared<ModelObject>("resource/user/Particle/", "yPlane.glb");


	D3D12App::Instance()->GenerateTextureBuffer(elecAnimationTexture.data(), "resource/user/Particle/elecParticle.png", 3, { 3,1 });

	for (int i = 0; i < m_particleArray.size(); ++i)
	{
		m_particleArray[i] = std::make_unique<DashLightingParticle>(m_model, elecAnimationTexture);
	}

	for (int i = 0; i < m_roadArray.size(); ++i)
	{
		m_roadArray[i] = std::make_unique<DashLightingRoad>(i);
	}


	m_lModel = m_model->m_model;
	m_lModel->m_meshes[0].material->texBuff[0] = elecAnimationTexture[0];
}

void PlayerDashLighting::Init(Vec3<float> *POS)
{
	m_pos = POS;
	m_timer = 0;
	m_roadIndex = -1;
}

void PlayerDashLighting::Update(bool ENABLE_FLAG, bool FEVER_FLAG)
{
	++m_timer;
	if (2 <= m_timer && FEVER_FLAG)
	{
		float size = 5.0f;
		Vec3<float>emittPos;
		emittPos.x = m_pos->x + KuroFunc::GetRand(-size, size);
		emittPos.y = KuroFunc::GetRand(-size, size);
		emittPos.z = m_pos->z + KuroFunc::GetRand(-size, size);
		m_particleArray[m_particleInitNum]->Init(emittPos);

		++m_particleInitNum;
		m_timer = 0;
	}
	if (m_particleArray.size() <= m_particleInitNum)
	{
		m_particleInitNum = 0;
	}

	for (int i = 0; i < m_particleArray.size(); ++i)
	{
		m_particleArray[i]->Update();
	}

	if (ENABLE_FLAG && !m_prevFlag)
	{
		++m_roadIndex;
		if (m_roadArray.size() <= m_roadIndex)
		{
			m_roadIndex = 0;
		}
		m_startPos = *m_pos;
		m_roadArray[m_roadIndex]->Init(m_startPos);
	}
	if (ENABLE_FLAG)
	{
		m_endPos = *m_pos;
	}
	m_prevFlag = ENABLE_FLAG;

	for (int i = 0; i < m_roadArray.size(); ++i)
	{
		m_roadArray[i]->Update(m_endPos, m_roadIndex, ENABLE_FLAG);
	}
}

void PlayerDashLighting::Draw(Camera &CAMERA)
{
	for (int i = 0; i < m_particleArray.size(); ++i)
	{
		m_particleArray[i]->Draw(CAMERA);
	}
	for (int i = 0; i < m_roadArray.size(); ++i)
	{
		m_roadArray[i]->Draw(CAMERA);
	}

	//Transform t;
	//t.SetPos({ 0.0f,10.0f,0.0f });
	//t.SetScale({ 50.0f,50.0f,50.0f });
	//DrawFunc_Append::DrawModel(m_lModel, t, RenderTargetSwitch(1.0f, 1.0f, 1.0f), true, false, nullptr,AlphaBlendMode_Trans);
}