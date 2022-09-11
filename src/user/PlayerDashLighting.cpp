#include "PlayerDashLighting.h"
#include"../engine/DrawFunc3D.h"

PlayerDashLighting::PlayerDashLighting()
{
}

void PlayerDashLighting::Init(Vec3<float> *POS)
{
	m_pos = POS;
	m_timer = 0;
	m_roadIndex = -1;
}

void PlayerDashLighting::Update(bool ENABLE_FLAG)
{
	++m_timer;
	if (2 <= m_timer && ENABLE_FLAG)
	{
		Vec3<float>emittPos;
		emittPos.x = m_pos->x + KuroFunc::GetRand(-3.0f, 3.0f);
		emittPos.y = KuroFunc::GetRand(-3.0f, 3.0f);
		emittPos.z = m_pos->z + KuroFunc::GetRand(-3.0f, 3.0f);
		m_particleArray[m_particleInitNum].Init(emittPos);

		++m_particleInitNum;
		m_timer = 0;
	}
	if (m_particleArray.size() <= m_particleInitNum)
	{
		m_particleInitNum = 0;
	}

	for (int i = 0; i < m_particleArray.size(); ++i)
	{
		m_particleArray[i].Update();
	}

	if (ENABLE_FLAG && !m_prevFlag)
	{
		++m_roadIndex;
		if (m_roadArray.size() <= m_roadIndex)
		{
			m_roadIndex = 0;
		}
		m_startPos = *m_pos;
		m_roadArray[m_roadIndex].Init(m_startPos);
	}
	if (ENABLE_FLAG)
	{
		m_endPos = *m_pos;
	}
	m_prevFlag = ENABLE_FLAG;

	for (int i = 0; i < m_roadArray.size(); ++i)
	{
		m_roadArray[i].Update(m_endPos, m_roadIndex);
	}
}

void PlayerDashLighting::Draw(Camera &CAMERA)
{
	for (int i = 0; i < m_particleArray.size(); ++i)
	{
		m_particleArray[i].Draw(CAMERA);
	}
	for (int i = 0; i < m_roadArray.size(); ++i)
	{
		m_roadArray[i].Draw(CAMERA);
	}
}
