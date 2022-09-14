#include "EnemyWaveMgr.h"
#include "EnemyWave.h"
#include"EnemyMgr.h"
#include "SlowMgr.h"
#include"EnemyWaveLoader.h"
#include"../engine/UsersInput.h"

EnemyWaveMgr::EnemyWaveMgr(const float& MapSize)
{
	/*===== �R���X�g���N�^ =====*/

	m_frameTimer = 0;

	// WAVE1���쐬�B
	std::shared_ptr<EnemyWave> wave1 = std::make_shared<EnemyWave>(0, false);


	wave1->AddEnemy(Vec3<float>(-30, 0, 30), Vec3<float>(), ENEMY_INFO::ID::STOPPING, 0, 300);
	wave1->AddEnemy(Vec3<float>(-30, 0, 0), Vec3<float>(), ENEMY_INFO::ID::STOPPING, 0, 180);
	wave1->AddEnemy(Vec3<float>(-30, 0, -30), Vec3<float>(), ENEMY_INFO::ID::STOPPING, 0, 300);
	wave1->AddEnemy(Vec3<float>(0, 0, -30), Vec3<float>(), ENEMY_INFO::ID::STOPPING, 0, 300);
	wave1->AddEnemy(Vec3<float>(0, 0, 30), Vec3<float>(), ENEMY_INFO::ID::STOPPING, 0, 300);
	wave1->AddEnemy(Vec3<float>(30, 0, -30), Vec3<float>(), ENEMY_INFO::ID::STOPPING, 0, 300);
	wave1->AddEnemy(Vec3<float>(30, 0, 30), Vec3<float>(), ENEMY_INFO::ID::STOPPING, 0, 300);


	wave1->AddEnemy(Vec3<float>(0, 0, 0), Vec3<float>(), ENEMY_INFO::ID::SHIELD, 600, 300);


	// WAVE1��ǉ��B
	m_waves.emplace_back(wave1);




	// WAVE2���쐬�B
	std::shared_ptr<EnemyWave> wave2 = std::make_shared<EnemyWave>(1500, false);


	wave2->AddEnemy(Vec3<float>(0, 0, 0), Vec3<float>(), ENEMY_INFO::ID::SHIELD, 0, 600);

	wave2->AddEnemy(Vec3<float>(0, 0, -60), Vec3<float>(), ENEMY_INFO::ID::UNION, 120, 180);
	wave2->AddEnemy(Vec3<float>(0, 0, 60), Vec3<float>(), ENEMY_INFO::ID::UNION, 120, 300);
	wave2->AddEnemy(Vec3<float>(60, 0, 0), Vec3<float>(), ENEMY_INFO::ID::UNION, 120, 180);
	wave2->AddEnemy(Vec3<float>(-60, 0, 0), Vec3<float>(), ENEMY_INFO::ID::UNION, 120, 300);

	// WAVE2��ǉ��B
	m_waves.emplace_back(wave2);




	// WAVE3���쐬�B
	std::shared_ptr<EnemyWave> wave3 = std::make_shared<EnemyWave>(3000, false);

	// �l��������^�񒆂Ɍ������ēG�𕦂�����B
	wave3->AddEnemy(Vec3<float>(0, 0, -100), -Vec3<float>(0, 0, -100).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 30, 600);
	wave3->AddEnemy(Vec3<float>(0, 0, 100), -Vec3<float>(0, 0, 100).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 30, 600);
	wave3->AddEnemy(Vec3<float>(100, 0, 0), -Vec3<float>(100, 0, 0).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 30, 600);
	wave3->AddEnemy(Vec3<float>(-100, 0, 0), -Vec3<float>(-100, 0, 0).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 30, 600);

	// �l��������^�񒆂Ɍ������ēG�𕦂�����B
	wave3->AddEnemy(Vec3<float>(0, 0, -100), -Vec3<float>(0, 0, -100).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 180, 600);
	wave3->AddEnemy(Vec3<float>(0, 0, 100), -Vec3<float>(0, 0, 100).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 180, 600);
	wave3->AddEnemy(Vec3<float>(100, 0, 0), -Vec3<float>(100, 0, 0).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 180, 600);
	wave3->AddEnemy(Vec3<float>(-100, 0, 0), -Vec3<float>(-100, 0, 0).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 180, 600);

	// �l��������^�񒆂Ɍ������ēG�𕦂�����B
	wave3->AddEnemy(Vec3<float>(0, 0, -100), -Vec3<float>(0, 0, -100).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 330, 600);
	wave3->AddEnemy(Vec3<float>(0, 0, 100), -Vec3<float>(0, 0, 100).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 330, 600);
	wave3->AddEnemy(Vec3<float>(100, 0, 0), -Vec3<float>(100, 0, 0).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 330, 600);
	wave3->AddEnemy(Vec3<float>(-100, 0, 0), -Vec3<float>(-100, 0, 0).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 330, 600);

	// WAVE3��ǉ��B
	m_waves.emplace_back(wave3);




	// WAVE4���쐬�B
	std::shared_ptr<EnemyWave> wave4 = std::make_shared<EnemyWave>(4500, false);

	// �G���L����\��
	wave4->AddEnemy(Vec3<float>(0, 0, 100), Vec3<float>(1, 0, 0), ENEMY_INFO::ID::ELEC_MUSHI, 0, 90000);

	// �G�������_���ɐF�X�ȕ����ɂ킩����B
	for (int index = 0; index < 5; ++index) {

		for (int generate = 0; generate < 10; ++generate) {

			wave4->AddEnemy(Vec3<float>(KuroFunc::GetRand(-100, 100), KuroFunc::GetRand(-100, 100), KuroFunc::GetRand(-100, 100)), Vec3<float>(1, 0, 0), ENEMY_INFO::ID::PLAYER_STRAIGHT, 180 + 120 * index, 90000);

		}
	}

	// WAVE4��ǉ��B
	m_waves.emplace_back(wave4);



	// WAVE5���쐬�B
	std::shared_ptr<EnemyWave> wave5 = std::make_shared<EnemyWave>(5700, true);


	float rad = 0;
	float length = MapSize;
	for (int index = 0; index < 50; ++index) {

		wave5->AddEnemy(Vec3<float>(cosf(rad) * length, 0.0f, sinf(rad) * length), Vec3<float>(), ENEMY_INFO::ID::COIN, 0, 900000);

		length -= MapSize / 50.0f;
		rad += 0.01f;

	}

	// WAVE5��ǉ��B
	m_waves.emplace_back(wave5);



	// WAVE6���쐬�B
	std::shared_ptr<EnemyWave> wave6 = std::make_shared<EnemyWave>(6500, true);


	// �l������ǔ��G���o���B
	wave6->AddEnemy(Vec3<float>(0, 0, -100), -Vec3<float>(0, 0, -100).GetNormal(), ENEMY_INFO::ID::TRACKING, 60, 600);
	wave6->AddEnemy(Vec3<float>(0, 0, 100), -Vec3<float>(0, 0, 100).GetNormal(), ENEMY_INFO::ID::TRACKING, 60, 600);
	wave6->AddEnemy(Vec3<float>(100, 0, 0), -Vec3<float>(100, 0, 0).GetNormal(), ENEMY_INFO::ID::TRACKING, 60, 600);
	wave6->AddEnemy(Vec3<float>(-100, 0, 0), -Vec3<float>(-100, 0, 0).GetNormal(), ENEMY_INFO::ID::TRACKING, 60, 600);

	// �l������ǔ��G���o���B
	wave6->AddEnemy(Vec3<float>(0, 0, -100), -Vec3<float>(0, 0, -100).GetNormal(), ENEMY_INFO::ID::TRACKING, 180, 600);
	wave6->AddEnemy(Vec3<float>(0, 0, 100), -Vec3<float>(0, 0, 100).GetNormal(), ENEMY_INFO::ID::TRACKING, 180, 600);
	wave6->AddEnemy(Vec3<float>(100, 0, 0), -Vec3<float>(100, 0, 0).GetNormal(), ENEMY_INFO::ID::TRACKING, 180, 600);
	wave6->AddEnemy(Vec3<float>(-100, 0, 0), -Vec3<float>(-100, 0, 0).GetNormal(), ENEMY_INFO::ID::TRACKING, 180, 600);

	// �l������ǔ��G���o���B
	wave6->AddEnemy(Vec3<float>(0, 0, -100), -Vec3<float>(0, 0, -100).GetNormal(), ENEMY_INFO::ID::TRACKING, 300, 600);
	wave6->AddEnemy(Vec3<float>(0, 0, 100), -Vec3<float>(0, 0, 100).GetNormal(), ENEMY_INFO::ID::TRACKING, 300, 600);
	wave6->AddEnemy(Vec3<float>(100, 0, 0), -Vec3<float>(100, 0, 0).GetNormal(), ENEMY_INFO::ID::TRACKING, 300, 600);
	wave6->AddEnemy(Vec3<float>(-100, 0, 0), -Vec3<float>(-100, 0, 0).GetNormal(), ENEMY_INFO::ID::TRACKING, 300, 600);

	// �l������ǔ��G���o���B
	wave6->AddEnemy(Vec3<float>(0, 0, -100), -Vec3<float>(0, 0, -100).GetNormal(), ENEMY_INFO::ID::TRACKING, 420, 600);
	wave6->AddEnemy(Vec3<float>(0, 0, 100), -Vec3<float>(0, 0, 100).GetNormal(), ENEMY_INFO::ID::TRACKING, 420, 600);
	wave6->AddEnemy(Vec3<float>(100, 0, 0), -Vec3<float>(100, 0, 0).GetNormal(), ENEMY_INFO::ID::TRACKING, 420, 600);
	wave6->AddEnemy(Vec3<float>(-100, 0, 0), -Vec3<float>(-100, 0, 0).GetNormal(), ENEMY_INFO::ID::TRACKING, 420, 600);

	// WAVE6��ǉ��B
	m_waves.emplace_back(wave6);




	// WAVE7���쐬�B
	std::shared_ptr<EnemyWave> wave7 = std::make_shared<EnemyWave>(6500, true);


	// �G���L�����o���B
	wave7->AddEnemy(Vec3<float>(0, 0, 50), Vec3<float>(), ENEMY_INFO::ID::ELEC_MUSHI, 0, 90000);

	// �O�����͂ނ悤�ɓ����G����������o���B
	for (int index = 0; index < 200; ++index) {

		wave1->AddEnemy(Vec3<float>(0.0f, 0.0f, 50.0f), Vec3<float>(1.0f, 0.0f, 0.0f), ENEMY_INFO::ID::TORUS_MOVE, 120, (60 * index));

	}

	// WAVE7��ǉ��B
	m_waves.emplace_back(wave7);




	// WAVE8���쐬�B
	std::shared_ptr<EnemyWave> wave8 = std::make_shared<EnemyWave>(6500, true);


	// ���e�����G���o��
	wave7->AddEnemy(Vec3<float>(0, 0, 0), Vec3<float>(), ENEMY_INFO::ID::SHIELD, 0, 60);


	// ���f�G��|��
	wave7->AddEnemy(Vec3<float>(0, 0, 0), Vec3<float>(), ENEMY_INFO::ID::PRESS, 360, 300);


	// WAVE8��ǉ��B
	m_waves.emplace_back(wave8);


	//wave1->AddEnemy(Vec3<float>(0.0f, 0.0f, 0.0f), Vec3<float>(0.0f, 0.0f, 0.0f), ENEMY_INFO::ID::UNION, 60);

	//for (int index = 0; index < 200; ++index) {

	//	wave1->AddEnemy(Vec3<float>(0.0f, 0.0f, 50.0f), Vec3<float>(1.0f, 0.0f, 0.0f), ENEMY_INFO::ID::TORUS_MOVE, 120, (60 * index));

	//}

	//wave1->AddEnemy(Vec3<float>(100.0f, 0.0f, 0.0f), Vec3<float>(0.0f, 0.0f, -1.0f), ENEMY_INFO::ID::ELEC_MUSHI, 600);

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
	//wave2->AddEnemy(Vec3<float>(50.0f, 0.0f, 0.0f), Vec3<float>(1.0f, 0.0f, 0.0f), ENEMY_INFO::ID::COIN, 60, 60);
	//m_waves.emplace_back(wave2);


	//m_waves = EnemyWaveLoader::Load("test", m_finalWaveTimer);
	//assert(!m_waves.empty());
}

void EnemyWaveMgr::Init(const int& FinalWaveTime, const int& FrameTimer)
{

	/*===== ���������� =====*/
	//assert(!m_waves.empty());

	m_frameTimer = FrameTimer;
	m_nowWaveIdx = 0;
	for (auto& w : m_waves)w->Stop();

	//�ꉞ�X�^�[�g���Ń\�[�g
	//std::sort(m_waves.begin(), m_waves.end(), [](std::shared_ptr<EnemyWave>& a, std::shared_ptr<EnemyWave>& b)
	//	{
	//		return a->WaveStartTime() < b->WaveStartTime();
	//	});

	//���
	m_waves[0]->Start();

	m_finalWaveTimer = FinalWaveTime;

	// ��1�E�F�[�u�̎��Ԃ��v�Z�B
	m_nowWaveMaxTimer = m_waves.front()->GetWaveEndFrameLocal();
}

void EnemyWaveMgr::Update(std::weak_ptr<EnemyMgr> EnemyMgr, const Vec3<float>& PlayerPos, const float& MapSize)
{
	/*===== �X�V���� =====*/

	if (SlowMgr::Instance()->m_slow <= 0.9f) return;

	// �t���[���̃^�C�}�[���X�V�B
	++m_frameTimer;

	// �E�F�[�u�̍X�V����
	m_waves[m_nowWaveIdx]->Update(EnemyMgr, PlayerPos, MapSize);

	//���̃E�F�[�u������ꍇ
	int nextWaveIdx = m_nowWaveIdx + 1;
	if (nextWaveIdx <= m_waves.size() - 1)
	{
		//���̃E�F�[�u�̊J�n���Ԃ�����
		bool nextStart = (m_waves[nextWaveIdx]->WaveStartTime() <= m_frameTimer)	//���Ԍo��
			|| (m_waves[m_nowWaveIdx]->IsAllEnemyAppear() && EnemyMgr.lock()->GetAllEnemyDead()||
				UsersInput::Instance()->KeyOnTrigger(DIK_2));	//�S�Ă̓G���o���؂��� && �S�����S���Ă���
		
		if (nextStart)
		{
			m_waves[m_nowWaveIdx]->Stop();
			EnemyMgr.lock()->AllDisappear();
			m_waves[nextWaveIdx]->Start();
			m_nowWaveIdx = nextWaveIdx;
		}
	}
	//�Ȃ��ꍇ�Ō�̃E�F�[�u�̎��Ԍv��
	else
	{
		if (0 < m_finalWaveTimer)
		{
			m_finalWaveTimer--;

			//�S�ẴE�F�[�u���I��
			if (m_finalWaveTimer <= 0)
			{
				EnemyMgr.lock()->AllDisappear();
			}
		}
	}

	if (m_waves[m_nowWaveIdx]->IsBounusStage())
	{
		bool debug = false;
	}

	// ���݂̃E�F�[�u�̎��Ԃ��v�Z�B
	m_nowWaveMaxTimer = m_waves[m_nowWaveIdx]->GetWaveEndFrameLocal();

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