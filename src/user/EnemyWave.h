#pragma once
#include "Vec.h"
#include <vector>
#include <memory>

class EnemyMgr;

namespace ENEMY_INFO {

	enum class ID {

		STOPPING,		// 動かない敵。
		STRAIGHT,		// まっすぐ動く敵。
		PLAYER_STRAIGHT,// 生成位置からプレイヤーの方向に真っ直ぐ動く敵。
		TRACKING,		// 追尾する敵。
		SHIELD,			// 盾持ちの敵。
		UNION,			// 集合体の敵。
		TORUS_MOVE,		// 中心を回るように動く。
		PRESS,			// 押し戻す敵。
		DASH,			// ダッシュする敵。
		ELEC_MUSHI,		// エレキ虫
		ID_COUNT,

	};

	struct SPAWN_INFO
	{

		Vec3<float> m_pos;
		Vec3<float> m_forwardVec;
		ID m_id;
		int m_generateFrame;

		SPAWN_INFO() :m_pos(Vec3<float>()), m_forwardVec(Vec3<float>(0, 0, 1)), m_id(ID::STOPPING), m_generateFrame(0) {};

	};


}

class EnemyWave {

private:

	/*===== メンバ変数 =====*/

	std::vector<ENEMY_INFO::SPAWN_INFO> m_enemys;	// 敵データ
	int m_waveStartFrame;							// ウェーブ開始フレーム
	int m_nowWaveFrame;								// このWaveが始まってからの経過時間。


public:

	/*===== メンバ関数 =====*/

	EnemyWave(const int& WaveStartFrame);
	void Init();
	void Update(std::weak_ptr<EnemyMgr> EnemyMgr, const Vec3<float>& PlayerPos, const float& MapSize);
	void AddEnemy(const ENEMY_INFO::SPAWN_INFO& EnemyInfo);
	void AddEnemy(const Vec3<float>& Pos, const Vec3<float>& ForwardVec, ENEMY_INFO::ID ID, const int& GenerateFrame);

};