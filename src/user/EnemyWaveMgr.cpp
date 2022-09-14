#include "EnemyWaveMgr.h"
#include "EnemyWave.h"
#include"EnemyMgr.h"
#include "SlowMgr.h"

EnemyWaveMgr::EnemyWaveMgr(const float &MapSize)
{
	/*===== コンストラクタ =====*/

	m_frameTimer = 0;

	// WAVE1を作成。
	std::shared_ptr<EnemyWave> wave1 = std::make_shared<EnemyWave>(0, false);



	//wave1->AddEnemy(Vec3<float>(0.0f, 0.0f, 0.0f), Vec3<float>(0.0f, 0.0f, 0.0f), ENEMY_INFO::ID::UNION, 60);

	for (int index = 0; index < 200; ++index) {

		wave1->AddEnemy(Vec3<float>(0.0f, 0.0f, 50.0f), Vec3<float>(1.0f, 0.0f, 0.0f), ENEMY_INFO::ID::TORUS_MOVE, 120, (60 * index));

	}

	//wave1->AddEnemy(Vec3<float>(100.0f, 0.0f, 0.0f), Vec3<float>(0.0f, 0.0f, -1.0f), ENEMY_INFO::ID::ELEC_MUSHI, 600);

	//wave1->AddEnemy(Vec3<float>(0.0f, 0.0f, 0.0f), Vec3<float>(0.0f, 0.0f, -1.0f), ENEMY_INFO::ID::PRESS, 60);

	//m_waves.emplace_back(wave1);

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
	//m_waves.emplace_back(wave1);

	//std::shared_ptr<EnemyWave> wave2 = std::make_shared<EnemyWave>(120, true);
	//wave2->AddEnemy(Vec3<float>(50.0f, 0.0f, 0.0f), Vec3<float>(1.0f, 0.0f, 0.0f), ENEMY_INFO::ID::COIN, 60);
	//m_waves.emplace_back(wave2);

	m_waves.emplace_back(std::make_shared<EnemyWave>(0, false));

}

void EnemyWaveMgr::Init(const int& FinalWaveTime, const int& FrameTimer)
{

	/*===== 初期化処理 =====*/
	assert(!m_waves.empty());

	m_frameTimer = FrameTimer;
	m_nowWaveIdx = 0;
	for (auto& w : m_waves)w->Stop();

	//一応スタート順でソート
	std::sort(m_waves.begin(), m_waves.end(), [](std::shared_ptr<EnemyWave>& a, std::shared_ptr<EnemyWave>& b)
		{
			return a->WaveStartTime() < b->WaveStartTime();
		});

	//一番
	m_waves[0]->Start();

	m_finalWaveTimer = FinalWaveTime;
}

void EnemyWaveMgr::Update(std::weak_ptr<EnemyMgr> EnemyMgr, const Vec3<float> &PlayerPos, const float &MapSize)
{
	/*===== 更新処理 =====*/

	if (SlowMgr::Instance()->m_slow <= 0.9f) return;

	// フレームのタイマーを更新。
	++m_frameTimer;

	// ウェーブの更新処理
	m_waves[m_nowWaveIdx]->Update(EnemyMgr, PlayerPos, MapSize);

	//次のウェーブがある場合
	int nextWaveIdx = m_nowWaveIdx + 1;
	if (nextWaveIdx <= m_waves.size() - 1)
	{
		//次のウェーブの開始時間を見る
		bool nextStart = m_waves[nextWaveIdx]->WaveStartTime() <= m_frameTimer;
		if (nextStart)
		{
			m_waves[m_nowWaveIdx]->Stop();
			EnemyMgr.lock()->AllDisappear();
			m_waves[nextWaveIdx]->Start();
			m_nowWaveIdx = nextWaveIdx;
		}
	}
	//ない場合最後のウェーブの時間計測
	else
	{
		if (0 < m_finalWaveTimer)
		{
			m_finalWaveTimer--;

			//全てのウェーブが終了
			if (m_finalWaveTimer <= 0)
			{

			}
		}
	}

	if (m_waves[m_nowWaveIdx]->IsBounusStage())
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
	return m_waves[m_nowWaveIdx]->IsBounusStage();
}