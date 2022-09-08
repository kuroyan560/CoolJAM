#pragma once
#include "Vec.h"
#include <vector>
#include <memory>

class EnemyWave;

class EnemyWaveMgr {

private:

	/*===== メンバ変数 =====*/

	std::vector<std::shared_ptr<EnemyWave>> m_waves;	// ウェーブ
	int m_frameTimer;									// フレームのタイマー


public:

	/*===== メンバ関数 =====*/

	EnemyWaveMgr();
	void Init();
	void Update(const Vec3<float>& PlayerPos);

};