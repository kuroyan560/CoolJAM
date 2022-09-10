#include "EnemyWave.h"
#include "EnemyMgr.h"

EnemyWave::EnemyWave(const int &WaveStartFrame, const bool &BounusStageFlag)
{

	/*===== �R���X�g���N�^ =====*/

	m_waveStartFrame = WaveStartFrame;
	m_nowWaveFrame = 0;
	m_isBounusStageFlag = BounusStageFlag;
	m_startWaveFlag = false;
	m_initFlag = false;
}

void EnemyWave::Init()
{
	/*===== ���������� =====*/

	m_nowWaveFrame = 0;
	m_startWaveFlag = true;
	m_initFlag = true;
}

void EnemyWave::Update(std::weak_ptr<EnemyMgr> EnemyMgr, const Vec3<float> &PlayerPos, const float &MapSize)
{
	/*===== �X�V���� ====*/
	if (!m_startWaveFlag) {
		return;
	}

	++m_nowWaveFrame;

	for (auto &index : m_enemys) {

		// �����t���[���������Ă��Ȃ������珈�����΂��B
		if (index.m_generateFrame != m_nowWaveFrame) continue;

		// ID�ɂ���ď����𕪂���B
		Vec3<float> forwardVec = index.m_forwardVec;
		if (index.m_id == ENEMY_INFO::ID::PLAYER_STRAIGHT) {

			forwardVec = Vec3<float>(PlayerPos - index.m_pos).GetNormal();

		}

		// ��������B
		EnemyMgr.lock()->Generate(PlayerPos, index.m_pos, forwardVec, static_cast<int>(index.m_id), MapSize);

	}

}

void EnemyWave::AddEnemy(const ENEMY_INFO::SPAWN_INFO &EnemyInfo)
{

	/*===== �G��ǉ� =====*/

	m_enemys.emplace_back(EnemyInfo);

}

void EnemyWave::AddEnemy(const Vec3<float> &Pos, const Vec3<float> &ForwardVec, ENEMY_INFO::ID ID, const int &GenerateFrame)
{

	/*===== �G��ǉ� =====*/

	ENEMY_INFO::SPAWN_INFO enemyInfo;

	enemyInfo.m_pos = Pos;
	enemyInfo.m_forwardVec = ForwardVec;
	enemyInfo.m_id = ID;
	enemyInfo.m_generateFrame = GenerateFrame;

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

void EnemyWave::Stop()
{
	m_startWaveFlag = false;
}

bool EnemyWave::IsStart()
{
	return m_initFlag;
}