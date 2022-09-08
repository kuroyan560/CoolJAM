#include "EnemyWaveMgr.h"
#include "EnemyWave.h"

EnemyWaveMgr::EnemyWaveMgr()
{

	/*===== コンストラクタ =====*/

	m_frameTimer = 0;

	const float MAP_SIZE = 100.0f;

	// WAVE1を作成。
	std::shared_ptr<EnemyWave> wave1 = std::make_shared<EnemyWave>(0);

	// 上下左右から真っ直ぐ来る敵
	wave1->AddEnemy(Vec3<float>(MAP_SIZE, 0, 0), Vec3<float>(-1, 0, 0), ENEMY_INFO::ID::STRAIGHT, 60);
	wave1->AddEnemy(Vec3<float>(-MAP_SIZE, 0, 0), Vec3<float>(1, 0, 0), ENEMY_INFO::ID::STRAIGHT, 60);
	wave1->AddEnemy(Vec3<float>(0, 0, MAP_SIZE), Vec3<float>(0, 0, -1), ENEMY_INFO::ID::STRAIGHT, 60);
	wave1->AddEnemy(Vec3<float>(0, 0, -MAP_SIZE), Vec3<float>(0, 0, 1), ENEMY_INFO::ID::STRAIGHT, 60);

	// 上下左右から真っ直ぐ来る敵
	wave1->AddEnemy(Vec3<float>(MAP_SIZE, 0, 0), Vec3<float>(-1, 0, 0), ENEMY_INFO::ID::STRAIGHT, 90);
	wave1->AddEnemy(Vec3<float>(-MAP_SIZE, 0, 0), Vec3<float>(1, 0, 0), ENEMY_INFO::ID::STRAIGHT, 90);
	wave1->AddEnemy(Vec3<float>(0, 0, MAP_SIZE), Vec3<float>(0, 0, -1), ENEMY_INFO::ID::STRAIGHT, 90);
	wave1->AddEnemy(Vec3<float>(0, 0, -MAP_SIZE), Vec3<float>(0, 0, 1), ENEMY_INFO::ID::STRAIGHT, 90);

	// 上下左右から真っ直ぐ来る敵
	wave1->AddEnemy(Vec3<float>(MAP_SIZE, 0, MAP_SIZE), Vec3<float>(-1, 0, -1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 120);
	wave1->AddEnemy(Vec3<float>(-MAP_SIZE, 0, -MAP_SIZE), Vec3<float>(1, 0, 1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 120);
	wave1->AddEnemy(Vec3<float>(-MAP_SIZE, 0, MAP_SIZE), Vec3<float>(1, 0, -1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 120);
	wave1->AddEnemy(Vec3<float>(MAP_SIZE, 0, -MAP_SIZE), Vec3<float>(-1, 0, 1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 120);

	// 斜めから中心に向かって来る敵
	wave1->AddEnemy(Vec3<float>(MAP_SIZE, 0, MAP_SIZE), Vec3<float>(-1, 0, -1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 150);
	wave1->AddEnemy(Vec3<float>(-MAP_SIZE, 0, -MAP_SIZE), Vec3<float>(1, 0, 1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 150);
	wave1->AddEnemy(Vec3<float>(-MAP_SIZE, 0, MAP_SIZE), Vec3<float>(1, 0, -1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 150);
	wave1->AddEnemy(Vec3<float>(MAP_SIZE, 0, -MAP_SIZE), Vec3<float>(-1, 0, 1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 150);

	// 斜めから中心に向かって来る敵
	wave1->AddEnemy(Vec3<float>(MAP_SIZE, 0, MAP_SIZE), Vec3<float>(-1, 0, -1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 180);
	wave1->AddEnemy(Vec3<float>(-MAP_SIZE, 0, -MAP_SIZE), Vec3<float>(1, 0, 1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 180);
	wave1->AddEnemy(Vec3<float>(-MAP_SIZE, 0, MAP_SIZE), Vec3<float>(1, 0, -1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 180);
	wave1->AddEnemy(Vec3<float>(MAP_SIZE, 0, -MAP_SIZE), Vec3<float>(-1, 0, 1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 180);

	// 斜めから中心に向かって来る敵
	wave1->AddEnemy(Vec3<float>(MAP_SIZE, 0, MAP_SIZE), Vec3<float>(-1, 0, -1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 210);
	wave1->AddEnemy(Vec3<float>(-MAP_SIZE, 0, -MAP_SIZE), Vec3<float>(1, 0, 1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 210);
	wave1->AddEnemy(Vec3<float>(-MAP_SIZE, 0, MAP_SIZE), Vec3<float>(1, 0, -1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 210);
	wave1->AddEnemy(Vec3<float>(MAP_SIZE, 0, -MAP_SIZE), Vec3<float>(-1, 0, 1).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 210);

	for (int index = 0; index < 20; ++index) {

		wave1->AddEnemy(Vec3<float>(), Vec3<float>(), ENEMY_INFO::ID::PLAYER_STRAIGHT, 360 + index * 20);

	}


	for (int index = 0; index < 1000; ++index) {

		wave1->AddEnemy(Vec3<float>(), Vec3<float>(), ENEMY_INFO::ID::PLAYER_STRAIGHT, 1000 + index * 20);

	}

	// WAVE1を追加。
	m_waves.emplace_back(wave1);

}

void EnemyWaveMgr::Init()
{

	/*===== 初期化処理 =====*/

	m_frameTimer = 0;

}

void EnemyWaveMgr::Update(std::weak_ptr<EnemyMgr> EnemyMgr, const Vec3<float>& PlayerPos, const float& MapSize)
{

	/*===== 更新処理 =====*/

	// フレームのタイマーを更新。
	++m_frameTimer;

	// ウェーブの更新処理
	for (auto& index : m_waves) {

		index->Update(EnemyMgr, m_frameTimer, PlayerPos, MapSize);

	}

}

void EnemyWaveMgr::AddWave(std::shared_ptr<EnemyWave> Wave)
{

	/*===== ウェーブを追加 =====*/

	m_waves.emplace_back(Wave);

}
