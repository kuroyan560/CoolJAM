#include "EnemyDeadLineParticle.h"
#include"../engine/Common/Angle.h"
#include"DrawFunc_Append.h"
#include "SlowMgr.h"

EnemyDeadLineParticle::EnemyDeadLineParticle() :m_length(4.0f), m_initFlag(0)
{
}

void EnemyDeadLineParticle::Init(const Vec3<float> &POS, float SPEED, int ANGLE, const Color &COLOR)
{
	m_pos = POS;
	m_vel = { cosf(Angle::ConvertToRadian(ANGLE)),0.0f,sinf(Angle::ConvertToRadian(ANGLE)) };
	m_vel *= SPEED;
	m_alpha = 255;
	m_dispappearTime = 255 / 60;
	m_initFlag = 1;
	m_rgb = {
		static_cast<int>(COLOR.m_r * 255.0f),
		static_cast<int>(COLOR.m_g * 255.0f),
		static_cast<int>(COLOR.m_b * 255.0f)
	};
}

void EnemyDeadLineParticle::Update()
{
	if (m_initFlag)
	{
		m_pos += m_vel * SlowMgr::Instance()->m_slow;

		//���̒���
		m_startPos = m_pos;
		m_endPos = m_pos + m_vel * m_length;

		m_alpha += -m_dispappearTime * SlowMgr::Instance()->m_slow;
		if (m_alpha <= 0)
		{
			m_alpha = 0;
			m_initFlag = 0;
		}
	}
}

void EnemyDeadLineParticle::Draw(Camera &CAMERA)
{
	if (m_initFlag)
	{
		//DrawFunc_Append::DrawLine(m_startPos, m_endPos, Color(), 1.8f, RenderTargetSwitch(m_alpha, 0.3f, 0.0f));
	}
}
