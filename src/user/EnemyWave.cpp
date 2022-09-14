#include "EnemyWave.h"
#include "EnemyMgr.h"
#include "SlowMgr.h"

EnemyWave::EnemyWave(const int &WaveStartFrame, const bool &BounusStageFlag)
{

	/*===== コンストラクタ =====*/

	m_waveStartFrame = WaveStartFrame;
	m_nowWaveFrame = 0;
	m_isBounusStageFlag = BounusStageFlag;
	m_startWaveFlag = false;
}

void EnemyWave::Init()
{
	/*===== 初期化処理 =====*/
	Stop();
}

void EnemyWave::Update(std::weak_ptr<EnemyMgr> EnemyMgr, const Vec3<float> &PlayerPos, const float &MapSize)
{
	/*===== 更新処理 ====*/
	if (!m_startWaveFlag) {
		return;
	}

	if (SlowMgr::Instance()->m_slow <= 0.9f) return;

	++m_nowWaveFrame;

	for (auto &index : m_enemys) {

		// 生成フレームが合っていなかったら処理を飛ばす。
		if (index.m_generateFrame != m_nowWaveFrame) continue;

		// IDによって処理を分ける。
		Vec3<float> forwardVec = index.m_forwardVec;
		if (index.m_id == ENEMY_INFO::ID::PLAYER_STRAIGHT) {

			forwardVec = Vec3<float>(PlayerPos - index.m_pos).GetNormal();

		}

		// 生成する。
		EnemyMgr.lock()->Generate(PlayerPos, index.m_pos, forwardVec, static_cast<int>(index.m_id), index.m_shotTimer, MapSize);

	}

}

void EnemyWave::AddEnemy(const ENEMY_INFO::SPAWN_INFO &EnemyInfo)
{

	/*===== 敵を追加 =====*/

	m_enemys.emplace_back(EnemyInfo);

}

void EnemyWave::AddEnemy(const Vec3<float> &Pos, const Vec3<float> &ForwardVec, ENEMY_INFO::ID ID, const int &GenerateFrame, const int& ShotTimer)
{

	/*===== 敵を追加 =====*/

	ENEMY_INFO::SPAWN_INFO enemyInfo;

	enemyInfo.m_pos = Pos;
	enemyInfo.m_forwardVec = ForwardVec;
	enemyInfo.m_id = ID;
	enemyInfo.m_generateFrame = GenerateFrame;
	enemyInfo.m_shotTimer = ShotTimer;

	m_enemys.emplace_back(enemyInfo);

}

bool EnemyWave::IsBounusStage()
{
	return m_isBounusStageFlag;
}

int EnemyWave::WaveStartTime()
{
	return m_waveStartFrame;
}

void EnemyWave::Start()
{
	m_startWaveFlag = true;
	m_nowWaveFrame = 0;
}

void EnemyWave::Stop()
{
	m_startWaveFlag = false;
}