#include "GrazeEmitter.h"
#include"KazCollisionHelper.h"

GrazeEmitter::GrazeEmitter()
{
}

void GrazeEmitter::Init(const Vec3<float>* POS, const Vec3<float> *VEL)
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
				Vec3<float>vel = *m_velPtr;
				vel.x *= -1.0f;
				vel.z *= -1.0f;
				m_particleArray[i].Init(*m_posPtr, vel, 10.0f);
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
