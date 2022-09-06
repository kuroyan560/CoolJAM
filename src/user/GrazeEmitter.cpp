#include "GrazeEmitter.h"
#include"KazCollisionHelper.h"

GrazeEmitter::GrazeEmitter()
{
}

void GrazeEmitter::Init(const Vec3<float>* POS, const float *VEL)
{
	m_posPtr = POS;
	m_velPtr = VEL;
	initTimer = 0;
}

void GrazeEmitter::Update(float MAP_SIZE)
{
	++initTimer;

	if (1 <= initTimer)
	{
		initTimer = 0;
		for (int i = 0; i < m_particleArray.size(); ++i)
		{
			if (!m_particleArray[i].IsAlive())
			{
				float centralDir = Angle::ConvertToRadian(90);
				//プレイヤーの真後ろに角度を合わせ、ランダムで放射状に発射されるようにする
				float angleLimit = 10.0f;
				float randAngle = KuroFunc::GetRand(-angleLimit, angleLimit);
				Vec3<float> radial = { cosf(*m_velPtr + centralDir + Angle::ConvertToRadian(randAngle)),0.0f,sinf(*m_velPtr + centralDir + Angle::ConvertToRadian(randAngle)) };
				//方向調整
				radial.z *= -1.0f;
				m_particleArray[i].Init(*m_posPtr, radial, 10.0f);
				break;
			}
		}
	}

	for (int i = 0; i < m_particleArray.size(); ++i)
	{
		m_particleArray[i].Update(MAP_SIZE);
	}
}

void GrazeEmitter::Draw(Camera& CAMERA)
{
	for (int i = 0; i < m_particleArray.size(); ++i)
	{
		m_particleArray[i].Draw(CAMERA);
	}
}
