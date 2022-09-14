#pragma once
#include "Vec.h"
#include <vector>
#include <memory>

class EnemyWave;
class EnemyMgr;

class EnemyWaveMgr {

private:
	friend class EnemyWaveEditor;
	friend class EnemyWaveLoader;

	/*===== メンバ変数 =====*/

	std::vector<std::shared_ptr<EnemyWave>> m_waves;	// ウェーブ
	float m_frameTimer;									// フレームのタイマー
	int m_nowWaveMaxTimer;
	int m_nowWaveIdx = -1;
	int m_finalWaveTimer = 0;

public:

	/*===== メンバ関数 =====*/

	EnemyWaveMgr(const float& MapSize);
	void Init(const int& FinalWaveTime, const int& FrameTimer = 0);
	void Update(std::weak_ptr<EnemyMgr> EnemyMgr, const Vec3<float>& PlayerPos, const float& MapSize);
	void AddWave(std::shared_ptr<EnemyWave> Wave);

	bool IsNowWaveBounusStage();
	bool EndAllWaves() { return m_finalWaveTimer <= 0; }
	int GetNowWaveMaxTimer() { return m_nowWaveMaxTimer; }
};