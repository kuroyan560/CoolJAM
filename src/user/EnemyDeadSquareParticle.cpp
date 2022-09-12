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
	m_dispappearTime = 60;
	m_initFlag = 1;
	m_angle = { 0,0,0 };
	m_color = COLOR;

	float minVel = -5.0f;
	float maxVel = 5.0f;
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


		const Vec2<float> LIMIT_LINE = { 200.0f,200.0f };
		if (m_pos.x <= LIMIT_LINE.x &&
			m_pos.z <= LIMIT_LINE.y &&
			-LIMIT_LINE.x <= m_pos.x &&
			-LIMIT_LINE.y <= m_pos.z)
		{
			m_initFlag = 1;
		}
		else
		{
			m_initFlag = 0;
			m_color.m_a = 0;
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
