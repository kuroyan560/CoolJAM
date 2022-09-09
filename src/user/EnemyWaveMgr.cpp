#include "EnemyWaveMgr.h"
#include "EnemyWave.h"

EnemyWaveMgr::EnemyWaveMgr()
{

	/*===== �R���X�g���N�^ =====*/

	m_frameTimer = 0;

	const float MAP_SIZE = 100.0f;

	// WAVE1���쐬�B
	std::shared_ptr<EnemyWave> wave1 = std::make_shared<EnemyWave>(0);

	// ���̓G
	wave1->AddEnemy(Vec3<float>(0, 0, 0), Vec3<float>(-1, 0, 0), ENEMY_INFO::ID::UNION, 120);

	// �㉺���E����^����������G
	for (int index = 0; index < 100; ++index) {

		//wave1->AddEnemy(Vec3<float>(MAP_SIZE / 2.0f, 0, 0), Vec3<float>(-1, 0, 0), ENEMY_INFO::ID::TRACKING, 60 + index * 600);
		//wave1->AddEnemy(Vec3<float>(-MAP_SIZE / 2.0f, 0, 0), Vec3<float>(1, 0, 0), ENEMY_INFO::ID::TRACKING, 60 + index * 600);
		//wave1->AddEnemy(Vec3<float>(0, 0, MAP_SIZE / 2.0f), Vec3<float>(0, 0, -1), ENEMY_INFO::ID::TRACKING, 60 + index * 600);
		//wave1->AddEnemy(Vec3<float>(0, 0, -MAP_SIZE / 2.0f), Vec3<float>(0, 0, 1), ENEMY_INFO::ID::TRACKING, 60 + index * 600);

		//wave1->AddEnemy(Vec3<float>(MAP_SIZE / 2.0f, 0, 0), Vec3<float>(-1, 0, 0), ENEMY_INFO::ID::STOPPING, 60 + index * 600);
		//wave1->AddEnemy(Vec3<float>(-MAP_SIZE / 2.0f, 0, 0), Vec3<float>(1, 0, 0), ENEMY_INFO::ID::STOPPING, 60 + index * 600);
		//wave1->AddEnemy(Vec3<float>(0, 0, MAP_SIZE / 2.0f), Vec3<float>(0, 0, -1), ENEMY_INFO::ID::STOPPING, 60 + index * 600);
		//wave1->AddEnemy(Vec3<float>(0, 0, -MAP_SIZE / 2.0f), Vec3<float>(0, 0, 1), ENEMY_INFO::ID::STOPPING, 60 + index * 600);

		//// �㉺���E����^����������G
		//wave1->AddEnemy(Vec3<float>(MAP_SIZE, 0, 0), Vec3<float>(-1, 0, 0), ENEMY_INFO::ID::STRAIGHT, 60 + index * 600);
		//wave1->AddEnemy(Vec3<float>(-MAP_SIZE, 0, 0), Vec3<float>(1, 0, 0), ENEMY_INFO::ID::STRAIGHT, 60 + index * 600);
		//wave1->AddEnemy(Vec3<float>(0, 0, MAP_SIZE), Vec3<float>(0, 0, -1), ENEMY_INFO::ID::STRAIGHT, 60 + index * 600);
		//wave1->AddEnemy(Vec3<float>(0, 0, -MAP_SIZE), Vec3<float>(0, 0, 1), ENEMY_INFO::ID::STRAIGHT, 60 + index * 600);

		//// �㉺���E����^����������G
		//wave1->AddEnemy(Vec3<float>(MAP_SIZE, 0, 0), Vec3<float>(-1, 0, 0), ENEMY_INFO::ID::STRAIGHT, 90 + index * 600);
		//wave1->AddEnemy(Vec3<float>(-MAP_SIZE, 0, 0), Vec3<float>(1, 0, 0), ENEMY_INFO::ID::STRAIGHT, 90 + index * 600);
		//wave1->AddEnemy(Vec3<float>(0, 0, MAP_SIZE), Vec3<float>(0, 0, -1), ENEMY_INFO::ID::STRAIGHT, 90 + index * 600);
		//wave1->AddEnemy(Vec3<float>(0, 0, -MAP_SIZE), Vec3<float>(0, 0, 1), ENEMY_INFO::ID::STRAIGHT, 90 + index * 600);

		//// �㉺���E����^����������G
		//wave1->AddEnemy(Vec3<float>(MAP_SIZE, 0, MAP_SIZE), Vec3<float>(-1, 0, -1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 120 + index * 600);
		//wave1->AddEnemy(Vec3<float>(-MAP_SIZE, 0, -MAP_SIZE), Vec3<float>(1, 0, 1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 120 + index * 600);
		//wave1->AddEnemy(Vec3<float>(-MAP_SIZE, 0, MAP_SIZE), Vec3<float>(1, 0, -1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 120 + index * 600);
		//wave1->AddEnemy(Vec3<float>(MAP_SIZE, 0, -MAP_SIZE), Vec3<float>(-1, 0, 1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 120 + index * 600);

		//// �΂߂��璆�S�Ɍ������ė���G
		//wave1->AddEnemy(Vec3<float>(MAP_SIZE, 0, MAP_SIZE), Vec3<float>(-1, 0, -1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 150 + index * 600);
		//wave1->AddEnemy(Vec3<float>(-MAP_SIZE, 0, -MAP_SIZE), Vec3<float>(1, 0, 1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 150 + index * 600);
		//wave1->AddEnemy(Vec3<float>(-MAP_SIZE, 0, MAP_SIZE), Vec3<float>(1, 0, -1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 150 + index * 600);
		//wave1->AddEnemy(Vec3<float>(MAP_SIZE, 0, -MAP_SIZE), Vec3<float>(-1, 0, 1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 150 + index * 600);

		//// �΂߂��璆�S�Ɍ������ė���G
		//wave1->AddEnemy(Vec3<float>(MAP_SIZE, 0, MAP_SIZE), Vec3<float>(-1, 0, -1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 180 + index * 600);
		//wave1->AddEnemy(Vec3<float>(-MAP_SIZE, 0, -MAP_SIZE), Vec3<float>(1, 0, 1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 180 + index * 600);
		//wave1->AddEnemy(Vec3<float>(-MAP_SIZE, 0, MAP_SIZE), Vec3<float>(1, 0, -1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 180 + index * 600);
		//wave1->AddEnemy(Vec3<float>(MAP_SIZE, 0, -MAP_SIZE), Vec3<float>(-1, 0, 1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 180 + index * 600);

		//// �΂߂��璆�S�Ɍ������ė���G
		//wave1->AddEnemy(Vec3<float>(MAP_SIZE, 0, MAP_SIZE), Vec3<float>(-1, 0, -1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 210 + index * 600);
		//wave1->AddEnemy(Vec3<float>(-MAP_SIZE, 0, -MAP_SIZE), Vec3<float>(1, 0, 1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 210 + index * 600);
		//wave1->AddEnemy(Vec3<float>(-MAP_SIZE, 0, MAP_SIZE), Vec3<float>(1, 0, -1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 210 + index * 600);
		//wave1->AddEnemy(Vec3<float>(MAP_SIZE, 0, -MAP_SIZE), Vec3<float>(-1, 0, 1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 210 + index * 600);

	}


	//for (int index = 0; index < 20; ++index) {

	//	wave1->AddEnemy(Vec3<float>(), Vec3<float>(), ENEMY_INFO::ID::PLAYER_STRAIGHT, 360 + index * 20);

	//}


	for (int index = 0; index < 100; ++index) {

		wave1->AddEnemy(Vec3<float>(), Vec3<float>(0,0,1), ENEMY_INFO::ID::TRACKING, 60 + index * 20);

	}

	// WAVE1��ǉ��B
	m_waves.emplace_back(wave1);

}

void EnemyWaveMgr::Init()
{

	/*===== ���������� =====*/

	m_frameTimer = 0;

}

void EnemyWaveMgr::Update(std::weak_ptr<EnemyMgr> EnemyMgr, const Vec3<float>& PlayerPos, const float& MapSize)
{

	/*===== �X�V���� =====*/

	// �t���[���̃^�C�}�[���X�V�B
	++m_frameTimer;

	// �E�F�[�u�̍X�V����
	for (auto& index : m_waves) {

		index->Update(EnemyMgr, m_frameTimer, PlayerPos, MapSize);

	}

}

void EnemyWaveMgr::AddWave(std::shared_ptr<EnemyWave> Wave)
{

	/*===== �E�F�[�u��ǉ� =====*/

	m_waves.emplace_back(Wave);

}
