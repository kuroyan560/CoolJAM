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

void GrazeEmitter::Update(float MAP_SIZE,bool HIT_FLAG)
{
	++initTimer;

	if (0 <= initTimer && HIT_FLAG)
	{
		initTimer = 0;
		for (int i = 0; i < m_particleArray.size(); ++i)
		{
			if (!m_particleArray[i].IsAlive())
			{
				float centralDir = Angle::ConvertToRadian(90);
				//プレイヤーの真後ろに角度を合わせ、ランダムで放射状に発射されるようにする
				float randAngle = 0.0f;//KuroFunc::GetRand(-static_cast<float>(RADIAL_LIMIT), static_cast<float>(RADIAL_LIMIT));
				float cosRadian = cosf(*m_velPtr + centralDir);
				float sinRadian = sinf(*m_velPtr + centralDir);
				Vec3<float>dir(cosRadian, 0.0f, -sinRadian);
				m_particleArray[i].Init(*m_posPtr, dir, 10.0f);
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
