#pragma once
#include "Vec.h"
#include <vector>
#include <memory>

class EnemyWave;
class EnemyMgr;

class EnemyWaveMgr {

private:
	friend class EnemyWaveEditor;

	/*===== メンバ変数 =====*/

	std::vector<std::shared_ptr<EnemyWave>> m_waves;	// ウェーブ
	int m_frameTimer;									// フレームのタイマー
	int m_nowWaveIdx = -1;

public:

	/*===== メンバ関数 =====*/

	EnemyWaveMgr(const float& MapSize);
	void Init(const int& FrameTimer = 0);
	void Update(std::weak_ptr<EnemyMgr> EnemyMgr, const Vec3<float>& PlayerPos, const float& MapSize);
	void AddWave(std::shared_ptr<EnemyWave> Wave);

	bool IsNowWaveBounusStage();
};