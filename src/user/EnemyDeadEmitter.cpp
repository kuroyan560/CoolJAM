#include "EnemyDeadEmitter.h"
#include"../engine/Common/KuroFunc.h"

EnemyDeadEmitter::EnemyDeadEmitter()
{

}

void EnemyDeadEmitter::Init(const Vec3<float> &POS)
{
	//l•ªŠ„‚µ‚ÄÅ’áŒÂ”‚ğo‚·
	int perNum = m_lineParticle.size() / 4;
	int nowPerCount = 0;
	for (int i = 0; i < m_lineParticle.size(); ++i)
	{
		if ((i + 1) % perNum == 0)
		{
			++nowPerCount;
		}
		int min = 0 + nowPerCount * 90;
		int max = min + 90;
		int random = KuroFunc::GetRand(min, max);
		m_lineParticle[i].Init(POS, random);
	}
}

void EnemyDeadEmitter::Update()
{
	for (int i = 0; i < m_lineParticle.size(); ++i)
	{
		m_lineParticle[i].Update();
	}
}

void EnemyDeadEmitter::Draw(Camera &CAMERA)
{
	for (int i = 0; i < m_lineParticle.size(); ++i)
	{
		m_lineParticle[i].Draw(CAMERA);
	}
}
