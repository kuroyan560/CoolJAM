#include"EnemyDeadSquareParticle.h"
#include"../engine/DrawFunc3D.h"

EnemyDeadSquareParticle::EnemyDeadSquareParticle()
{
}

void EnemyDeadSquareParticle::Init(const Vec3<float> &POS, float SPEED, int ANGLE, const Color &COLOR)
{
	m_pos = POS;
	m_vel = { cosf(Angle::ConvertToRadian(ANGLE)),0.0f,sinf(Angle::ConvertToRadian(ANGLE)) };
	m_vel *= SPEED;
	m_color.m_a = 1.0f;
	m_dispappearTime = 1.0f / 30.0f;
	m_initFlag = 1;
	m_angle = { 0,0,0 };

	float minVel = -30.5f;
	float maxVel = 30.5f;
	m_angleVel = {
		KuroFunc::GetRand(minVel, maxVel),
		KuroFunc::GetRand(minVel, maxVel),
		KuroFunc::GetRand(minVel, maxVel)
	};

}

void EnemyDeadSquareParticle::Update()
{
	if (m_initFlag)
	{
		m_pos += m_vel;
		m_angle += m_angleVel;

		m_color.m_a += -m_dispappearTime;
		if (m_color.m_a <= 0)
		{
			m_color.m_a = 0;
			m_initFlag = 0;
		}
	}
}

void EnemyDeadSquareParticle::Draw(Camera &CAMERA)
{
	if (m_initFlag)
	{
		//DrawFunc3D::DrawNonShadingModel(m_model, m_transform, CAMERA);
	}
}
