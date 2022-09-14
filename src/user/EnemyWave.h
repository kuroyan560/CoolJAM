#pragma once
#include "Vec.h"
#include <vector>
#include <memory>
#include<string>
#include<array>

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
		COIN,			// コイン
		ID_COUNT,

	};

	static const std::string& GetIDName(ID Id)
	{
		static const std::array<std::string, static_cast<int>(ID::ID_COUNT)> NAME_LIST =
		{
			"STOPPING",
			"STRAIGHT",
			"PLAYER_STRAIGHT",
			"TRACKING",
			"SHIELD",
			"UNION",
			"TORUS_MOVE",
			"PRESS",
			"DASH",
			"ELEC_MUSI",
			"COIN"
		};
		return NAME_LIST[static_cast<int>(Id)];
	}

	struct SPAWN_INFO
	{

		Vec3<float> m_pos;
		Vec3<float> m_forwardVec;
		ID m_id;
		int m_generateFrame;
		int m_shotTimer;

		SPAWN_INFO() :m_pos(Vec3<float>()), m_forwardVec(Vec3<float>(0, 0, 1)), m_id(ID::STOPPING), m_generateFrame(0), m_shotTimer(0) {};

	};


}

class EnemyWave {

private:
	friend class EnemyWaveEditor;
	friend class EnemyWaveLoader;

	/*===== メンバ変数 =====*/

	std::vector<ENEMY_INFO::SPAWN_INFO> m_enemys;	// 敵データ
	int m_waveStartFrame;							// ウェーブ開始フレーム
	float m_nowWaveFrame;							// このWaveが始まってからの経過時間。
	bool m_isBounusStageFlag;
	bool m_startWaveFlag;

public:

	/*===== メンバ関数 =====*/

	EnemyWave(const int& WaveStartFrame, const bool& BounusStageFlag);
	void Init();
	void Update(std::weak_ptr<EnemyMgr> EnemyMgr, const Vec3<float>& PlayerPos, const float& MapSize);
	void AddEnemy(const ENEMY_INFO::SPAWN_INFO& EnemyInfo);
	void AddEnemy(const Vec3<float>& Pos, const Vec3<float>& ForwardVec, ENEMY_INFO::ID ID, const int& GenerateFrame, const int& ShotTimer);

	void Start();
	void Stop();
	bool IsBounusStage();
	int WaveStartTime();

	int GetWaveStartFrame() { return m_waveStartFrame; }
	int GetWaveEndFrameLocal() { return m_enemys.back().m_generateFrame; }

};

