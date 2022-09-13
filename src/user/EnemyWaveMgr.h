#pragma once
#include "D3D12data.h"
#include "Vec.h"
#include <vector>
#include <memory>

class EnemyWave;
class EnemyMgr;
class WaveUI;

class EnemyWaveMgr {

private:
	friend class EnemyWaveEditor;
	friend class EnemyWaveLoader;

	/*===== �����o�ϐ� =====*/

	std::vector<std::shared_ptr<EnemyWave>> m_waves;	// �E�F�[�u
	int m_frameTimer;									// �t���[���̃^�C�}�[
	int m_nowWaveIdx = -1;
	int m_finalWaveTimer = 0;

	Vec2<float> m_waveCount;

	std::shared_ptr<WaveUI> m_waveUI;


public:

	/*===== �����o�֐� =====*/

	EnemyWaveMgr(const float& MapSize);
	void Init(const int& FinalWaveTime, const int& FrameTimer = 0);
	void Update(std::weak_ptr<EnemyMgr> EnemyMgr, const Vec3<float>& PlayerPos, const float& MapSize);
	void AddWave(std::shared_ptr<EnemyWave> Wave);

	void Draw();

	bool IsNowWaveBounusStage();
	bool EndAllWaves() { return m_finalWaveTimer <= 0; }
};