#include "EnemyWave.h"
#include "EnemyMgr.h"
#include "SlowMgr.h"

EnemyWave::EnemyWave(const int& WaveStartFrame, const bool& BounusStageFlag)
{

	/*===== �R���X�g���N�^ =====*/

	m_waveStartFrame = WaveStartFrame;
	m_nowWaveFrame = 0;
	m_isBounusStageFlag = BounusStageFlag;
	m_startWaveFlag = false;
}

void EnemyWave::Init()
{
	/*===== ���������� =====*/
	Stop();
}

void EnemyWave::Update(std::weak_ptr<EnemyMgr> EnemyMgr, const Vec3<float>& PlayerPos, const float& MapSize)
{
	/*===== �X�V���� ====*/
	if (!m_startWaveFlag) {
		return;
	}

	if (SlowMgr::Instance()->m_slow <= 0.9f) return;

	++m_nowWaveFrame;

	for (auto& index : m_enemys) {

		// �����t���[���������Ă��Ȃ������珈�����΂��B
		if (index.m_generateFrame != m_nowWaveFrame) continue;

		// ID�ɂ���ď����𕪂���B
		Vec3<float> forwardVec = index.m_forwardVec;
		if (index.m_id == ENEMY_INFO::ID::PLAYER_STRAIGHT) {

			forwardVec = Vec3<float>(PlayerPos - index.m_pos).GetNormal();

		}

		// ��������B
		EnemyMgr.lock()->Generate(PlayerPos, index.m_pos, forwardVec, static_cast<int>(index.m_id), index.m_shotTimer, MapSize);
		m_generateCount++;
	}

}

void EnemyWave::AddEnemy(const ENEMY_INFO::SPAWN_INFO& EnemyInfo)
{

	/*===== �G��ǉ� =====*/

	m_enemys.emplace_back(EnemyInfo);

}

void EnemyWave::AddEnemy(const Vec3<float>& Pos, const Vec3<float>& ForwardVec, ENEMY_INFO::ID ID, const int& GenerateFrame, const int& ShotTimer)
{

	/*===== �G��ǉ� =====*/

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

int EnemyWave::GetWaveEndFrameLocal()
{
	if (m_enemys.empty())return 0;

	int min = 100000000;

	for (auto& e : m_enemys)
	{
		if (e.m_generateFrame < min)min = e.m_generateFrame;
	}

	return min;
}

void EnemyWave::Start()
{
	m_startWaveFlag = true;
	m_nowWaveFrame = 0;
	m_generateCount = 0;
}

void EnemyWave::Stop()
{
	m_startWaveFlag = false;
}