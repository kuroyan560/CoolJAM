#include "EnemyWaveMgr.h"
#include "EnemyWave.h"

EnemyWaveMgr::EnemyWaveMgr(const float &MapSize)
{

	nowWaveCount = -1;

	/*===== �R���X�g���N�^ =====*/

	m_frameTimer = 0;

	// WAVE1���쐬�B
	std::shared_ptr<EnemyWave> wave1 = std::make_shared<EnemyWave>(0, false);

	wave1->AddEnemy(Vec3<float>(0.0f, 0.0f, 50.0f), Vec3<float>(1.0f, 0.0f, 0.0f), ENEMY_INFO::ID::STOPPING, 60);
	wave1->AddEnemy(Vec3<float>(-50.0f, 0.0f, 0.0f), Vec3<float>(0.0f, 0.0f, 1.0f), ENEMY_INFO::ID::STOPPING, 60);
	wave1->AddEnemy(Vec3<float>(0.0f, 0.0f, -50.0f), Vec3<float>(-1.0f, 0.0f, 0.0f), ENEMY_INFO::ID::STOPPING, 60);
	wave1->AddEnemy(Vec3<float>(50.0f, 0.0f, 0.0f), Vec3<float>(0.0f, 0.0f, -1.0f), ENEMY_INFO::ID::STOPPING, 60);

	//wave1->AddEnemy(Vec3<float>(0.0f, 0.0f, 0.0f), Vec3<float>(0.0f, 0.0f, 0.0f), ENEMY_INFO::ID::UNION, 60);

	for (int index = 0; index < 200; ++index) {

		//wave1->AddEnemy(Vec3<float>(0.0f, 0.0f, 0.0f), Vec3<float>(0.0f, 0.0f, 0.0f), ENEMY_INFO::ID::TRACKING, 60 + index * 60);

	}


	wave1->AddEnemy(Vec3<float>(0.0f, 0.0f, 0.0f), Vec3<float>(0.0f, 0.0f, -1.0f), ENEMY_INFO::ID::PRESS, 60);

	//// �}�b�v�̎l���ɓG��z�u�B
	//wave1->AddEnemy(Vec3<float>(MapSize / 2.0f, 0.0f, 0.0f), Vec3<float>(0.0f, 0.0f, 0.0f), ENEMY_INFO::ID::STOPPING, 30);
	//wave1->AddEnemy(Vec3<float>(-MapSize / 2.0f, 0.0f, 0.0f), Vec3<float>(0.0f, 0.0f, 0.0f), ENEMY_INFO::ID::STOPPING, 30);
	//wave1->AddEnemy(Vec3<float>(0.0f, 0.0f, MapSize / 2.0f), Vec3<float>(0.0f, 0.0f, 0.0f), ENEMY_INFO::ID::STOPPING, 30);
	//wave1->AddEnemy(Vec3<float>(0.0f, 0.0f, -MapSize / 2.0f), Vec3<float>(0.0f, 0.0f, 0.0f), ENEMY_INFO::ID::STOPPING, 30);

	//// �}�b�v�̎l���ɓG��z�u�B
	//wave1->AddEnemy(Vec3<float>(MapSize / 2.0f, 0.0f, MapSize / 2.0f).GetNormal() * (MapSize / 2.0f), Vec3<float>(0.0f, 0.0f, 0.0f), ENEMY_INFO::ID::STOPPING, 210);
	//wave1->AddEnemy(Vec3<float>(-MapSize / 2.0f, 0.0f, -MapSize / 2.0f).GetNormal() * (MapSize / 2.0f), Vec3<float>(0.0f, 0.0f, 0.0f), ENEMY_INFO::ID::STOPPING, 210);
	//wave1->AddEnemy(Vec3<float>(-MapSize / 2.0f, 0.0f, MapSize / 2.0f).GetNormal() * (MapSize / 2.0f), Vec3<float>(0.0f, 0.0f, 0.0f), ENEMY_INFO::ID::STOPPING, 210);
	//wave1->AddEnemy(Vec3<float>(MapSize / 2.0f, 0.0f, -MapSize / 2.0f).GetNormal() * (MapSize / 2.0f), Vec3<float>(0.0f, 0.0f, 0.0f), ENEMY_INFO::ID::STOPPING, 210);


	// WAVE1��ǉ��B
	m_waves.emplace_back(wave1);

}

void EnemyWaveMgr::Init()
{

	/*===== ���������� =====*/

	m_frameTimer = 0;

}

void EnemyWaveMgr::Update(std::weak_ptr<EnemyMgr> EnemyMgr, const Vec3<float> &PlayerPos, const float &MapSize)
{

	/*===== �X�V���� =====*/

	// �t���[���̃^�C�}�[���X�V�B
	++m_frameTimer;

	// �E�F�[�u�̍X�V����
	for (int i = 0; i < m_waves.size(); ++i)
	{
		//Wave�̊J�n�̏�����
		bool isStartFlag = !m_waves[i]->IsStart() && m_waves[i]->WaveStartTime() <= m_frameTimer;
		if (isStartFlag)
		{
			m_waves[i]->Init();
			++nowWaveCount;
		}
		//����Wave���J�n���ꂽ��OWave�̐����������~�߂�
		if (isStartFlag && 0 <= (i - 1))
		{
			m_waves[i - 1]->Stop();
		}


		m_waves[i]->Update(EnemyMgr, PlayerPos, MapSize);
	}

	if (m_waves[nowWaveCount]->IsBounusStage())
	{
		bool debug = false;
	}
}

void EnemyWaveMgr::AddWave(std::shared_ptr<EnemyWave> Wave)
{

	/*===== �E�F�[�u��ǉ� =====*/

	m_waves.emplace_back(Wave);

}

bool EnemyWaveMgr::IsNowWaveBounusStage()
{
	return m_waves[nowWaveCount]->IsBounusStage();
}