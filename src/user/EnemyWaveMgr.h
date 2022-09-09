#pragma once
#include "Vec.h"
#include <vector>
#include <memory>

class EnemyWave;
class EnemyMgr;

class EnemyWaveMgr {

private:

	/*===== メンバ変数 =====*/

	std::vector<std::shared_ptr<EnemyWave>> m_waves;	// ウェーブ
	int m_frameTimer;									// フレームのタイマー


public:

	/*===== メンバ関数 =====*/

	EnemyWaveMgr(const float& MapSize);
	void Init();
	void Update(std::weak_ptr<EnemyMgr> EnemyMgr, const Vec3<float>& PlayerPos, const float& MapSize);
	void AddWave(std::shared_ptr<EnemyWave> Wave);

};