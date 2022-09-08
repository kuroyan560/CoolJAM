#include "EnemyDeadLineParticle.h"
#include"../engine/DrawFunc3D.h"
#include"../engine/Common/Angle.h"

EnemyDeadLineParticle::EnemyDeadLineParticle() :m_length(2.0f), m_initFlag(false)
{
}

void EnemyDeadLineParticle::Init(const Vec3<float> &POS,float SPEED, int ANGLE)
{
	m_pos = POS;
	m_vel = { cosf(Angle::ConvertToRadian(ANGLE)),0.0f,sinf(Angle::ConvertToRadian(ANGLE)) };
	m_vel *= SPEED;
	m_alpha = 255;
	m_dispappearTime = 255 / 30;
	m_initFlag = true;
}

void EnemyDeadLineParticle::Update()
{
	if (m_initFlag)
	{
		m_pos += m_vel;

		//ê¸ÇÃí∑Ç≥
		m_startPos = m_pos;
		m_endPos = m_pos + m_vel * m_length;

		m_alpha += -m_dispappearTime;
		if (m_alpha <= 0)
		{
			m_alpha = 0;
			m_initFlag = false;
		}
	}
}

void EnemyDeadLineParticle::Draw(Camera &CAMERA)
{
	if (m_initFlag)
	{
		DrawFunc3D::DrawLine(CAMERA, m_startPos, m_endPos, Color(255, 255, 255, m_alpha), 0.3f);
	}
}
