#include "EnemyDeadEmitter.h"
#include"../engine/Common/KuroFunc.h"

EnemyDeadEmitter::EnemyDeadEmitter()
{
	std::shared_ptr<Model>model = Importer::Instance()->LoadModel("resource/user/", "enemy.glb");
	for (int i = 0; i < m_lineParticle.size(); ++i)
	{
		m_SquareParticle[i] = std::make_unique<EnemyDeadSquareParticle>(model);
	}
}

void EnemyDeadEmitter::Init(const Vec3<float> &POS)
{
	//l•ªŠ„‚µ‚ÄÅ’áŒÂ”‚ğo‚·
	const int PER_NUM = m_lineParticle.size() / 4;
	int nowPerCount = 0;
	for (int i = 0; i < m_lineParticle.size(); ++i)
	{
		if ((i + 1) % PER_NUM == 0)
		{
			++nowPerCount;
		}
		int min = 0 + nowPerCount * 90;
		int max = min + 90;
		int random = KuroFunc::GetRand(min, max);
		m_lineParticle[i].Init(POS, KuroFunc::GetRand(1.3f, 2.0f), random);
	}

	nowPerCount = 0;
	for (int i = 0; i < m_SquareParticle.size(); ++i)
	{
		if ((i + 1) % PER_NUM == 0)
		{
			++nowPerCount;
		}
		int min = 0 + nowPerCount * 90;
		int max = min + 90;
		int random = KuroFunc::GetRand(min, max);
		m_SquareParticle[i]->Init(POS, KuroFunc::GetRand(0.4f, 1.0f), random);
	}
}

void EnemyDeadEmitter::Update()
{
	for (int i = 0; i < m_lineParticle.size(); ++i)
	{
		m_lineParticle[i].Update();
	}
	for (int i = 0; i < m_SquareParticle.size(); ++i)
	{
		m_SquareParticle[i]->Update();
	}
}

void EnemyDeadEmitter::Draw(Camera &CAMERA)
{
	for (int i = 0; i < m_lineParticle.size(); ++i)
	{
		m_lineParticle[i].Draw(CAMERA);
	}
	for (int i = 0; i < m_SquareParticle.size(); ++i)
	{
		m_SquareParticle[i]->Draw(CAMERA);
	}
}
