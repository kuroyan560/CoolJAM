#include "EnemyWaveMgr.h"
#include "EnemyWave.h"

EnemyWaveMgr::EnemyWaveMgr(const float &MapSize)
{

	nowWaveCount = -1;

	/*===== コンストラクタ =====*/

	m_frameTimer = 0;

	// WAVE1を作成。
	std::shared_ptr<EnemyWave> wave1 = std::make_shared<EnemyWave>(0, false);

	wave1->AddEnemy(Vec3<float>(0.0f, 0.0f, 50.0f), Vec3<float>(1.0f, 0.0f, 0.0f), ENEMY_INFO::ID::ELEC_MUSHI, 60);
	wave1->AddEnemy(Vec3<float>(-50.0f, 0.0f, 0.0f), Vec3<float>(0.0f, 0.0f, 1.0f), ENEMY_INFO::ID::ELEC_MUSHI, 60);
	wave1->AddEnemy(Vec3<float>(0.0f, 0.0f, -50.0f), Vec3<float>(-1.0f, 0.0f, 0.0f), ENEMY_INFO::ID::ELEC_MUSHI, 60);
	wave1->AddEnemy(Vec3<float>(50.0f, 0.0f, 0.0f), Vec3<float>(0.0f, 0.0f, -1.0f), ENEMY_INFO::ID::ELEC_MUSHI, 60);

	//wave1->AddEnemy(Vec3<float>(0.0f, 0.0f, 0.0f), Vec3<float>(0.0f, 0.0f, 0.0f), ENEMY_INFO::ID::UNION, 60);

	for (int index = 0; index < 200; ++index) {

		//wave1->AddEnemy(Vec3<float>(0.0f, 0.0f, 0.0f), Vec3<float>(0.0f, 0.0f, 0.0f), ENEMY_INFO::ID::TRACKING, 60 + index * 60);

	}


	wave1->AddEnemy(Vec3<float>(0.0f, 0.0f, 0.0f), Vec3<float>(0.0f, 0.0f, -1.0f), ENEMY_INFO::ID::PRESS, 60);

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

void EnemyWaveMgr::Update(std::weak_ptr<EnemyMgr> EnemyMgr, const Vec3<float> &PlayerPos, const float &MapSize)
{

	/*===== 更新処理 =====*/

	// フレームのタイマーを更新。
	++m_frameTimer;

	// ウェーブの更新処理
	for (int i = 0; i < m_waves.size(); ++i)
	{
		//Waveの開始の初期化
		bool isStartFlag = !m_waves[i]->IsStart() && m_waves[i]->WaveStartTime() <= m_frameTimer;
		if (isStartFlag)
		{
			m_waves[i]->Init();
			++nowWaveCount;
		}
		//次のWaveが開始されたら前Waveの生成処理を止める
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

	/*===== ウェーブを追加 =====*/

	m_waves.emplace_back(Wave);

}

bool EnemyWaveMgr::IsNowWaveBounusStage()
{
	return m_waves[nowWaveCount]->IsBounusStage();
}