#include "EnemyWaveMgr.h"
#include "EnemyWave.h"

EnemyWaveMgr::EnemyWaveMgr(const float &MapSize)
{
	/*===== �R���X�g���N�^ =====*/

	m_frameTimer = 0;

	// WAVE1���쐬�B
	std::shared_ptr<EnemyWave> wave1 = std::make_shared<EnemyWave>(0, false);



	//wave1->AddEnemy(Vec3<float>(0.0f, 0.0f, 0.0f), Vec3<float>(0.0f, 0.0f, 0.0f), ENEMY_INFO::ID::UNION, 60);

	/*for (int index = 0; index < 200; ++index) {

		wave1->AddEnemy(Vec3<float>(0.0f, 0.0f, 50.0f), Vec3<float>(1.0f, 0.0f, 0.0f), ENEMY_INFO::ID::TORUS_MOVE, (30 * index));

	}*/
	for (int index = 0; index < 200; ++index) {

		wave1->AddEnemy(Vec3<float>(0.0f, 0.0f, 50.0f), Vec3<float>(1.0f, 0.0f, 0.0f), ENEMY_INFO::ID::TORUS_MOVE, (30 * index));

	}

	wave1->AddEnemy(Vec3<float>(100.0f, 0.0f, 0.0f), Vec3<float>(0.0f, 0.0f, -1.0f), ENEMY_INFO::ID::ELEC_MUSHI, 600);

	//wave1->AddEnemy(Vec3<float>(0.0f, 0.0f, 0.0f), Vec3<float>(0.0f, 0.0f, -1.0f), ENEMY_INFO::ID::PRESS, 60);

	//m_waves.emplace_back(wave1);

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
	//m_waves.emplace_back(wave1);

	//std::shared_ptr<EnemyWave> wave2 = std::make_shared<EnemyWave>(120, true);
	//wave2->AddEnemy(Vec3<float>(50.0f, 0.0f, 0.0f), Vec3<float>(1.0f, 0.0f, 0.0f), ENEMY_INFO::ID::COIN, 60);
	//m_waves.emplace_back(wave2);

}

void EnemyWaveMgr::Init(const int& FrameTimer)
{

	/*===== ���������� =====*/

	m_frameTimer = FrameTimer;
	m_nowWaveIdx = 0;
	for (auto& w : m_waves)w->Stop();

	//�ꉞ�X�^�[�g���Ń\�[�g
	std::sort(m_waves.begin(), m_waves.end(), [](std::shared_ptr<EnemyWave>& a, std::shared_ptr<EnemyWave>& b)
		{
			return a->WaveStartTime() < b->WaveStartTime();
		});
}

void EnemyWaveMgr::Update(std::weak_ptr<EnemyMgr> EnemyMgr, const Vec3<float> &PlayerPos, const float &MapSize)
{
	/*===== �X�V���� =====*/

	assert(!m_waves.empty());

	// �t���[���̃^�C�}�[���X�V�B
	++m_frameTimer;

	// �E�F�[�u�̍X�V����
	m_waves[m_nowWaveIdx]->Update(EnemyMgr, PlayerPos, MapSize);

	//���̃E�F�[�u������ꍇ
	int nextWaveIdx = m_nowWaveIdx + 1;
	if (nextWaveIdx <= m_waves.size() - 1)
	{
		//���̃E�F�[�u�̊J�n���Ԃ�����
		bool nextStart = m_waves[nextWaveIdx]->WaveStartTime() <= m_frameTimer;
		if (nextStart)
		{
			m_waves[m_nowWaveIdx]->Stop();
			m_waves[nextWaveIdx]->Start();
			m_nowWaveIdx = nextWaveIdx;
		}
	}

	if (m_waves[m_nowWaveIdx]->IsBounusStage())
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
	return m_waves[m_nowWaveIdx]->IsBounusStage();
}