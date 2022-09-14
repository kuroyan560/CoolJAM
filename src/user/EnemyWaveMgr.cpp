#include "EnemyWaveMgr.h"
#include "EnemyWave.h"
#include"EnemyMgr.h"
#include "SlowMgr.h"
#include"EnemyWaveLoader.h"
#include"../engine/UsersInput.h"

EnemyWaveMgr::EnemyWaveMgr(const float& MapSize)
{
	/*===== コンストラクタ =====*/

	m_frameTimer = 0;

	// WAVE1を作成。
	std::shared_ptr<EnemyWave> wave1 = std::make_shared<EnemyWave>(0, false);


	wave1->AddEnemy(Vec3<float>(-30, 0, 30), Vec3<float>(), ENEMY_INFO::ID::STOPPING, 0, 300);
	wave1->AddEnemy(Vec3<float>(-30, 0, 0), Vec3<float>(), ENEMY_INFO::ID::STOPPING, 0, 180);
	wave1->AddEnemy(Vec3<float>(-30, 0, -30), Vec3<float>(), ENEMY_INFO::ID::STOPPING, 0, 300);
	wave1->AddEnemy(Vec3<float>(0, 0, -30), Vec3<float>(), ENEMY_INFO::ID::STOPPING, 0, 300);
	wave1->AddEnemy(Vec3<float>(0, 0, 30), Vec3<float>(), ENEMY_INFO::ID::STOPPING, 0, 300);
	wave1->AddEnemy(Vec3<float>(30, 0, -30), Vec3<float>(), ENEMY_INFO::ID::STOPPING, 0, 300);
	wave1->AddEnemy(Vec3<float>(30, 0, 30), Vec3<float>(), ENEMY_INFO::ID::STOPPING, 0, 300);


	wave1->AddEnemy(Vec3<float>(0, 0, 0), Vec3<float>(), ENEMY_INFO::ID::SHIELD, 600, 300);


	// WAVE1を追加。
	m_waves.emplace_back(wave1);




	// WAVE2を作成。
	std::shared_ptr<EnemyWave> wave2 = std::make_shared<EnemyWave>(1500, false);


	wave2->AddEnemy(Vec3<float>(0, 0, 0), Vec3<float>(), ENEMY_INFO::ID::SHIELD, 0, 600);

	wave2->AddEnemy(Vec3<float>(0, 0, -60), Vec3<float>(), ENEMY_INFO::ID::UNION, 120, 180);
	wave2->AddEnemy(Vec3<float>(0, 0, 60), Vec3<float>(), ENEMY_INFO::ID::UNION, 120, 300);
	wave2->AddEnemy(Vec3<float>(60, 0, 0), Vec3<float>(), ENEMY_INFO::ID::UNION, 120, 180);
	wave2->AddEnemy(Vec3<float>(-60, 0, 0), Vec3<float>(), ENEMY_INFO::ID::UNION, 120, 300);

	// WAVE2を追加。
	m_waves.emplace_back(wave2);




	// WAVE3を作成。
	std::shared_ptr<EnemyWave> wave3 = std::make_shared<EnemyWave>(3000, false);

	// 四方向から真ん中に向かって敵を沸かせる。
	wave3->AddEnemy(Vec3<float>(0, 0, -100), -Vec3<float>(0, 0, -100).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 30, 600);
	wave3->AddEnemy(Vec3<float>(0, 0, 100), -Vec3<float>(0, 0, 100).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 30, 600);
	wave3->AddEnemy(Vec3<float>(100, 0, 0), -Vec3<float>(100, 0, 0).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 30, 600);
	wave3->AddEnemy(Vec3<float>(-100, 0, 0), -Vec3<float>(-100, 0, 0).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 30, 600);

	// 四方向から真ん中に向かって敵を沸かせる。
	wave3->AddEnemy(Vec3<float>(0, 0, -100), -Vec3<float>(0, 0, -100).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 180, 600);
	wave3->AddEnemy(Vec3<float>(0, 0, 100), -Vec3<float>(0, 0, 100).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 180, 600);
	wave3->AddEnemy(Vec3<float>(100, 0, 0), -Vec3<float>(100, 0, 0).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 180, 600);
	wave3->AddEnemy(Vec3<float>(-100, 0, 0), -Vec3<float>(-100, 0, 0).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 180, 600);

	// 四方向から真ん中に向かって敵を沸かせる。
	wave3->AddEnemy(Vec3<float>(0, 0, -100), -Vec3<float>(0, 0, -100).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 330, 600);
	wave3->AddEnemy(Vec3<float>(0, 0, 100), -Vec3<float>(0, 0, 100).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 330, 600);
	wave3->AddEnemy(Vec3<float>(100, 0, 0), -Vec3<float>(100, 0, 0).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 330, 600);
	wave3->AddEnemy(Vec3<float>(-100, 0, 0), -Vec3<float>(-100, 0, 0).GetNormal(), ENEMY_INFO::ID::STRAIGHT, 330, 600);

	// WAVE3を追加。
	m_waves.emplace_back(wave3);




	// WAVE4を作成。
	std::shared_ptr<EnemyWave> wave4 = std::make_shared<EnemyWave>(4500, false);

	// エレキ虫を表示
	wave4->AddEnemy(Vec3<float>(0, 0, 100), Vec3<float>(1, 0, 0), ENEMY_INFO::ID::ELEC_MUSHI, 0, 90000);

	// 敵をランダムに色々な方向にわかせる。
	for (int index = 0; index < 5; ++index) {

		for (int generate = 0; generate < 10; ++generate) {

			wave4->AddEnemy(Vec3<float>(KuroFunc::GetRand(-100, 100), 0, KuroFunc::GetRand(-100, 100)), Vec3<float>(1, 0, 0), ENEMY_INFO::ID::PLAYER_STRAIGHT, 180 + 120 * index, 90000);

		}
	}

	// WAVE4を追加。
	m_waves.emplace_back(wave4);



	// WAVE5を作成。
	std::shared_ptr<EnemyWave> wave5 = std::make_shared<EnemyWave>(5300, true);


	float rad = 0;
	float length = MapSize;
	for (int index = 0; index < 50; ++index) {

		wave5->AddEnemy(Vec3<float>(cosf(rad), 0.0f, sinf(rad)).GetNormal() * length, Vec3<float>(), ENEMY_INFO::ID::COIN, 0, 900000);

		length -= MapSize / 50.0f;
		rad += 0.1f;

	}

	// WAVE5を追加。
	m_waves.emplace_back(wave5);



	// WAVE6を作成。
	std::shared_ptr<EnemyWave> wave6 = std::make_shared<EnemyWave>(6000, true);


	// 四方から追尾敵を出す。
	wave6->AddEnemy(Vec3<float>(0, 0, -100), -Vec3<float>(0, 0, -100).GetNormal(), ENEMY_INFO::ID::TRACKING, 60, 600);
	wave6->AddEnemy(Vec3<float>(0, 0, 100), -Vec3<float>(0, 0, 100).GetNormal(), ENEMY_INFO::ID::TRACKING, 60, 600);
	wave6->AddEnemy(Vec3<float>(100, 0, 0), -Vec3<float>(100, 0, 0).GetNormal(), ENEMY_INFO::ID::TRACKING, 60, 600);
	wave6->AddEnemy(Vec3<float>(-100, 0, 0), -Vec3<float>(-100, 0, 0).GetNormal(), ENEMY_INFO::ID::TRACKING, 60, 600);

	// 四方から追尾敵を出す。
	wave6->AddEnemy(Vec3<float>(0, 0, -100), -Vec3<float>(0, 0, -100).GetNormal(), ENEMY_INFO::ID::TRACKING, 180, 600);
	wave6->AddEnemy(Vec3<float>(0, 0, 100), -Vec3<float>(0, 0, 100).GetNormal(), ENEMY_INFO::ID::TRACKING, 180, 600);
	wave6->AddEnemy(Vec3<float>(100, 0, 0), -Vec3<float>(100, 0, 0).GetNormal(), ENEMY_INFO::ID::TRACKING, 180, 600);
	wave6->AddEnemy(Vec3<float>(-100, 0, 0), -Vec3<float>(-100, 0, 0).GetNormal(), ENEMY_INFO::ID::TRACKING, 180, 600);

	// 四方から追尾敵を出す。
	wave6->AddEnemy(Vec3<float>(0, 0, -100), -Vec3<float>(0, 0, -100).GetNormal(), ENEMY_INFO::ID::TRACKING, 300, 600);
	wave6->AddEnemy(Vec3<float>(0, 0, 100), -Vec3<float>(0, 0, 100).GetNormal(), ENEMY_INFO::ID::TRACKING, 300, 600);
	wave6->AddEnemy(Vec3<float>(100, 0, 0), -Vec3<float>(100, 0, 0).GetNormal(), ENEMY_INFO::ID::TRACKING, 300, 600);
	wave6->AddEnemy(Vec3<float>(-100, 0, 0), -Vec3<float>(-100, 0, 0).GetNormal(), ENEMY_INFO::ID::TRACKING, 300, 600);

	// 四方から追尾敵を出す。
	wave6->AddEnemy(Vec3<float>(0, 0, -100), -Vec3<float>(0, 0, -100).GetNormal(), ENEMY_INFO::ID::TRACKING, 420, 600);
	wave6->AddEnemy(Vec3<float>(0, 0, 100), -Vec3<float>(0, 0, 100).GetNormal(), ENEMY_INFO::ID::TRACKING, 420, 600);
	wave6->AddEnemy(Vec3<float>(100, 0, 0), -Vec3<float>(100, 0, 0).GetNormal(), ENEMY_INFO::ID::TRACKING, 420, 600);
	wave6->AddEnemy(Vec3<float>(-100, 0, 0), -Vec3<float>(-100, 0, 0).GetNormal(), ENEMY_INFO::ID::TRACKING, 420, 600);

	// WAVE6を追加。
	m_waves.emplace_back(wave6);




	// WAVE7を作成。
	std::shared_ptr<EnemyWave> wave7 = std::make_shared<EnemyWave>(7000, true);


	// エレキ虫を出す。
	wave7->AddEnemy(Vec3<float>(0, 0, 50), Vec3<float>(), ENEMY_INFO::ID::ELEC_MUSHI, 0, 90000);

	// 外周を囲むように動く敵をたくさん出す。
	for (int index = 0; index < 200; ++index) {

		wave7->AddEnemy(Vec3<float>(0.0f, 0.0f, 50.0f), Vec3<float>(1.0f, 0.0f, 0.0f), ENEMY_INFO::ID::TORUS_MOVE, 120, (60 * index));

	}

	// WAVE7を追加。
	m_waves.emplace_back(wave7);




	// WAVE8を作成。
	std::shared_ptr<EnemyWave> wave8 = std::make_shared<EnemyWave>(8000, true);


	// 超弾を撃つ敵を出す
	wave8->AddEnemy(Vec3<float>(0, 0, 0), Vec3<float>(), ENEMY_INFO::ID::SHIELD, 0, 60);


	// お素敵を倒す
	wave8->AddEnemy(Vec3<float>(0, 0, 0), Vec3<float>(), ENEMY_INFO::ID::PRESS, 360, 300);


	// WAVE8を追加。
	m_waves.emplace_back(wave8);




	// WAVE9を作成。
	std::shared_ptr<EnemyWave> wave9 = std::make_shared<EnemyWave>(9000, true);


	for (int index = 0; index < 10; ++index) {

		wave9->AddEnemy(Vec3<float>(0, 0, -100.0f + index * 10.0f), Vec3<float>(-1, 0, 0), ENEMY_INFO::ID::STRAIGHT, 0, 9000);

	}


	// WAVE9を追加。
	m_waves.emplace_back(wave9);




	// WAVE10を作成。
	std::shared_ptr<EnemyWave> wave10 = std::make_shared<EnemyWave>(10000, true);


	// 真ん中にエレキ虫を出す。
	wave10->AddEnemy(Vec3<float>(0, 0, 0), Vec3<float>(0, 0, 0), ENEMY_INFO::ID::ELEC_MUSHI, 0, 9000);


	// 周りから大量の敵を出す。
	rad = 0;
	float addRad = DirectX::XM_2PI / 10.0f;
	for (int count = 0; count < 10; ++count) {
		for (int index = 0; index < 10; ++index) {

			Vec3<float> generatePos = Vec3<float>(cosf(rad), 0.0f, sinf(rad)) * 140.0f;
			wave10->AddEnemy(generatePos, -generatePos.GetNormal(), ENEMY_INFO::ID::STRAIGHT, 120 * count, 90000);

			rad += addRad;

		}
	}


	// WAVE10を追加。
	m_waves.emplace_back(wave10);


	//wave1->AddEnemy(Vec3<float>(0.0f, 0.0f, 0.0f), Vec3<float>(0.0f, 0.0f, 0.0f), ENEMY_INFO::ID::UNION, 60);

	//for (int index = 0; index < 200; ++index) {

	//	wave1->AddEnemy(Vec3<float>(0.0f, 0.0f, 50.0f), Vec3<float>(1.0f, 0.0f, 0.0f), ENEMY_INFO::ID::TORUS_MOVE, 120, (60 * index));

	//}

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
	//wave2->AddEnemy(Vec3<float>(50.0f, 0.0f, 0.0f), Vec3<float>(1.0f, 0.0f, 0.0f), ENEMY_INFO::ID::COIN, 60, 60);
	//m_waves.emplace_back(wave2);


	//m_waves = EnemyWaveLoader::Load("test", m_finalWaveTimer);
	//assert(!m_waves.empty());
}

void EnemyWaveMgr::Init(const int& FinalWaveTime, const int& FrameTimer)
{

	/*===== 初期化処理 =====*/
	//assert(!m_waves.empty());

	m_frameTimer = FrameTimer;
	m_nowWaveIdx = 0;
	for (auto& w : m_waves)w->Stop();

	//一応スタート順でソート
	//std::sort(m_waves.begin(), m_waves.end(), [](std::shared_ptr<EnemyWave>& a, std::shared_ptr<EnemyWave>& b)
	//	{
	//		return a->WaveStartTime() < b->WaveStartTime();
	//	});

	//一番
	m_waves[0]->Start();

	m_finalWaveTimer = FinalWaveTime;

	// 第1ウェーブの時間を計算。
	m_nowWaveMaxTimer = m_waves.front()->GetWaveEndFrameLocal();
}

void EnemyWaveMgr::Update(std::weak_ptr<EnemyMgr> EnemyMgr, const Vec3<float>& PlayerPos, const float& MapSize)
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
		bool nextStart = (m_waves[nextWaveIdx]->WaveStartTime() <= m_frameTimer)	//時間経過
			|| (m_waves[m_nowWaveIdx]->IsAllEnemyAppear() && EnemyMgr.lock()->GetAllEnemyDead() ||
				UsersInput::Instance()->KeyOnTrigger(DIK_2));	//全ての敵を出し切った && 全員死亡している

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
				EnemyMgr.lock()->AllDisappear();
			}
		}
	}

	if (m_waves[m_nowWaveIdx]->IsBounusStage())
	{
		bool debug = false;
	}

	// 現在のウェーブの時間を計算。
	m_nowWaveMaxTimer = m_waves[m_nowWaveIdx]->GetWaveEndFrameLocal();

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