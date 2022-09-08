#include "EnemyWaveMgr.h"
#include "EnemyWave.h"

EnemyWaveMgr::EnemyWaveMgr()
{

	/*===== �R���X�g���N�^ =====*/

	m_frameTimer = 0;

	const float MAP_SIZE = 100.0f;

	// WAVE1���쐬�B
	std::shared_ptr<EnemyWave> wave1 = std::make_shared<EnemyWave>(0);

	// �㉺���E����^����������G
	wave1->AddEnemy(Vec3<float>(MAP_SIZE, 0, 0), Vec3<float>(-1, 0, 0), ENEMY_INFO::ID::STRAIGHT, 60);
	wave1->AddEnemy(Vec3<float>(-MAP_SIZE, 0, 0), Vec3<float>(1, 0, 0), ENEMY_INFO::ID::STRAIGHT, 60);
	wave1->AddEnemy(Vec3<float>(0, 0, MAP_SIZE), Vec3<float>(0, 0, -1), ENEMY_INFO::ID::STRAIGHT, 60);
	wave1->AddEnemy(Vec3<float>(0, 0, -MAP_SIZE), Vec3<float>(0, 0, 1), ENEMY_INFO::ID::STRAIGHT, 60);

	// �㉺���E����^����������G
	wave1->AddEnemy(Vec3<float>(MAP_SIZE, 0, 0), Vec3<float>(-1, 0, 0), ENEMY_INFO::ID::STRAIGHT, 90);
	wave1->AddEnemy(Vec3<float>(-MAP_SIZE, 0, 0), Vec3<float>(1, 0, 0), ENEMY_INFO::ID::STRAIGHT, 90);
	wave1->AddEnemy(Vec3<float>(0, 0, MAP_SIZE), Vec3<float>(0, 0, -1), ENEMY_INFO::ID::STRAIGHT, 90);
	wave1->AddEnemy(Vec3<float>(0, 0, -MAP_SIZE), Vec3<float>(0, 0, 1), ENEMY_INFO::ID::STRAIGHT, 90);

	// �㉺���E����^����������G
	wave1->AddEnemy(Vec3<float>(MAP_SIZE, 0, MAP_SIZE), Vec3<float>(-1, 0, -1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 120);
	wave1->AddEnemy(Vec3<float>(-MAP_SIZE, 0, -MAP_SIZE), Vec3<float>(1, 0, 1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 120);
	wave1->AddEnemy(Vec3<float>(-MAP_SIZE, 0, MAP_SIZE), Vec3<float>(1, 0, -1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 120);
	wave1->AddEnemy(Vec3<float>(MAP_SIZE, 0, -MAP_SIZE), Vec3<float>(-1, 0, 1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 120);

	// �΂߂��璆�S�Ɍ������ė���G
	wave1->AddEnemy(Vec3<float>(MAP_SIZE, 0, MAP_SIZE), Vec3<float>(-1, 0, -1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 150);
	wave1->AddEnemy(Vec3<float>(-MAP_SIZE, 0, -MAP_SIZE), Vec3<float>(1, 0, 1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 150);
	wave1->AddEnemy(Vec3<float>(-MAP_SIZE, 0, MAP_SIZE), Vec3<float>(1, 0, -1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 150);
	wave1->AddEnemy(Vec3<float>(MAP_SIZE, 0, -MAP_SIZE), Vec3<float>(-1, 0, 1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 150);

	// �΂߂��璆�S�Ɍ������ė���G
	wave1->AddEnemy(Vec3<float>(MAP_SIZE, 0, MAP_SIZE), Vec3<float>(-1, 0, -1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 180);
	wave1->AddEnemy(Vec3<float>(-MAP_SIZE, 0, -MAP_SIZE), Vec3<float>(1, 0, 1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 180);
	wave1->AddEnemy(Vec3<float>(-MAP_SIZE, 0, MAP_SIZE), Vec3<float>(1, 0, -1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 180);
	wave1->AddEnemy(Vec3<float>(MAP_SIZE, 0, -MAP_SIZE), Vec3<float>(-1, 0, 1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 180);

	// �΂߂��璆�S�Ɍ������ė���G
	wave1->AddEnemy(Vec3<float>(MAP_SIZE, 0, MAP_SIZE), Vec3<float>(-1, 0, -1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 210);
	wave1->AddEnemy(Vec3<float>(-MAP_SIZE, 0, -MAP_SIZE), Vec3<float>(1, 0, 1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 210);
	wave1->AddEnemy(Vec3<float>(-MAP_SIZE, 0, MAP_SIZE), Vec3<float>(1, 0, -1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 210);
	wave1->AddEnemy(Vec3<float>(MAP_SIZE, 0, -MAP_SIZE), Vec3<float>(-1, 0, 1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 210);


	wave1->AddEnemy(Vec3<float>(), Vec3<float>(), ENEMY_INFO::ID::PLAYER_STRAIGHT, 360);
	wave1->AddEnemy(Vec3<float>(), Vec3<float>(), ENEMY_INFO::ID::PLAYER_STRAIGHT, 380);
	wave1->AddEnemy(Vec3<float>(), Vec3<float>(), ENEMY_INFO::ID::PLAYER_STRAIGHT, 400);
	wave1->AddEnemy(Vec3<float>(), Vec3<float>(), ENEMY_INFO::ID::PLAYER_STRAIGHT, 420);
	wave1->AddEnemy(Vec3<float>(), Vec3<float>(), ENEMY_INFO::ID::PLAYER_STRAIGHT, 440);
	wave1->AddEnemy(Vec3<float>(), Vec3<float>(), ENEMY_INFO::ID::PLAYER_STRAIGHT, 460);
	wave1->AddEnemy(Vec3<float>(), Vec3<float>(), ENEMY_INFO::ID::PLAYER_STRAIGHT, 480);
	wave1->AddEnemy(Vec3<float>(), Vec3<float>(), ENEMY_INFO::ID::PLAYER_STRAIGHT, 500);
	wave1->AddEnemy(Vec3<float>(), Vec3<float>(), ENEMY_INFO::ID::PLAYER_STRAIGHT, 520);
	wave1->AddEnemy(Vec3<float>(), Vec3<float>(), ENEMY_INFO::ID::PLAYER_STRAIGHT, 540);
	wave1->AddEnemy(Vec3<float>(), Vec3<float>(), ENEMY_INFO::ID::PLAYER_STRAIGHT, 560);

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
