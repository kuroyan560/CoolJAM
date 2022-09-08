#include "EnemyWave.h"
#include "EnemyMgr.h"

EnemyWave::EnemyWave(const int& WaveStartFrame)
{

	/*===== コンストラクタ =====*/

	m_waveStartFrame = WaveStartFrame;

}

void EnemyWave::Update(std::weak_ptr<EnemyMgr> EnemyMgr, const int& NowFrame, const Vec3<float>& PlayerPos, const float& MapSize)
{

	/*===== 更新処理 ====*/

	for (auto& index : m_enemys) {

		// 生成フレームが合っていなかったら処理を飛ばす。
		if (index.m_generateFrame != NowFrame) continue;

		// IDによって処理を分ける。
		Vec3<float> forwardVec = index.m_forwardVec;
		if (index.m_id == ENEMY_INFO::ID::PLAYER_STRAIGHT) {

			forwardVec = Vec3<float>(PlayerPos - index.m_pos).GetNormal();

		}

		// 生成する。
		EnemyMgr.lock()->Generate(PlayerPos, index.m_pos, forwardVec, static_cast<int>(index.m_id), MapSize);

	}

}

void EnemyWave::AddEnemy(const ENEMY_INFO::SPAWN_INFO& EnemyInfo)
{

	/*===== 敵を追加 =====*/

	m_enemys.emplace_back(EnemyInfo);

}

void EnemyWave::AddEnemy(const Vec3<float>& Pos, const Vec3<float>& ForwardVec, ENEMY_INFO::ID ID, const int& GenerateFrame)
{

	/*===== 敵を追加 =====*/

	ENEMY_INFO::SPAWN_INFO enemyInfo;

	enemyInfo.m_pos = Pos;
	enemyInfo.m_forwardVec = ForwardVec;
	enemyInfo.m_id = ID;
	enemyInfo.m_generateFrame = GenerateFrame;

	m_enemys.emplace_back(enemyInfo);

}
