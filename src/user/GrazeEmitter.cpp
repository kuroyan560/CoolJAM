#include "GrazeEmitter.h"
#include"KazCollisionHelper.h"

GrazeEmitter::GrazeEmitter()
{
}

void GrazeEmitter::Init(const Vec3<float>* POS)
{
	m_posPtr = POS;
}

void GrazeEmitter::Update()
{
	for (int i = 0; i < m_particleArray.size(); ++i)
	{
		m_particleArray[i].Update();
	}
}

void GrazeEmitter::Draw(Camera& CAMERA)
{
	for (int i = 0; i < m_particleArray.size(); ++i)
	{
		m_particleArray[i].Draw(CAMERA);
	}
}
