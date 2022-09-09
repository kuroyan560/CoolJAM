#include "EnemyWaveMgr.h"
#include "EnemyWave.h"

EnemyWaveMgr::EnemyWaveMgr(const float& MapSize)
{

	/*===== コンストラクタ =====*/

	m_frameTimer = 0;

	// WAVE1を作成。
	std::shared_ptr<EnemyWave> wave1 = std::make_shared<EnemyWave>(0);

	wave1->AddEnemy(Vec3<float>(0.0f, 0.0f, 50.0f), Vec3<float>(1.0f, 0.0f, 0.0f), ENEMY_INFO::ID::TORUS_MOVE, 60);
	wave1->AddEnemy(Vec3<float>(-50.0f, 0.0f, 0.0f), Vec3<float>(0.0f, 0.0f, 1.0f), ENEMY_INFO::ID::TORUS_MOVE, 60);
	wave1->AddEnemy(Vec3<float>(0.0f, 0.0f, -50.0f), Vec3<float>(-1.0f, 0.0f, 0.0f), ENEMY_INFO::ID::TORUS_MOVE, 60);
	wave1->AddEnemy(Vec3<float>(50.0f, 0.0f, 0.0f), Vec3<float>(0.0f, 0.0f, -1.0f), ENEMY_INFO::ID::TORUS_MOVE, 60);

	wave1->AddEnemy(Vec3<float>(0.0f, 0.0f, 0.0f), Vec3<float>(0.0f, 0.0f, 0.0f), ENEMY_INFO::ID::PRESS, 60);

	//// マップの四方に敵を配置。
	//wave1->AddEnemy(Vec3<float>(MapSize / 2.0f, 0.0f, 0.0f), Vec3<float>(0.0f, 0.0f, 0.0f), ENEMY_INFO::ID::STOPPING, 30);
	//wave1->AddEnemy(Vec3<float>(-MapSize / 2.0f, 0.0f, 0.0f), Vec3<float>(0.0f, 0.0f, 0.0f), ENEMY_INFO::ID::STOPPING, 30);
	//wave1->AddEnemy(Vec3<float>(0.0f, 0.0f, MapSize / 2.0f), Vec3<float>(0.0f, 0.0f, 0.0f), ENEMY_INFO::ID::STOPPING, 30);
	//wave1->AddEnemy(Vec3<float>(0.0f, 0.0f, -MapSize / 2.0f), Vec3<float>(0.0f, 0.0f, 0.0f), ENEMY_INFO::ID::STOPPING, 30);

	//// マップの四方に敵を配置。
	//wave1->AddEnemy(Vec3<float>(MapSize / 2.0f, 0.0f, MapSize / 2.0f).GetNormal() * (MapSize / 2.0f), Vec3<float>(0.0f, 0.0f, 0.0f), ENEMY_INFO::ID::STOPPING, 210);
	//wave1->AddEnemy(Vec3<float>(-MapSize / 2.0f, 0.0f, -MapSize / 2.0f).GetNormal() * (MapSize / 2.0f), Vec3<float>(0.0f, 0.0f, 0.0f), ENEMY_INFO::ID::STOPPING, 210);
	//wave1->AddEnemy(Vec3<float>(-MapSize / 2.0f, 0.0f, MapSize / 2.0f).GetNormal() * (MapSize / 2.0f), Vec3<float>(0.0f, 0.0f, 0.0f), ENEMY_INFO::ID::STOPPING, 210);
	//wave1->AddEnemy(Vec3<float>(MapSize / 2.0f, 0.0f, -MapSize / 2.0f).GetNormal() * (MapSize / 2.0f), Vec3<float>(0.0f, 0.0f, 0.0f), ENEMY_INFO::ID::STOPPING, 210);


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

		index->Update(EnemyMgr, PlayerPos, MapSize);

	}

}

void EnemyWaveMgr::AddWave(std::shared_ptr<EnemyWave> Wave)
{

	/*===== ウェーブを追加 =====*/

	m_waves.emplace_back(Wave);

}
