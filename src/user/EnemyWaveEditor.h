#pragma once
#include"Singleton.h"
#include<memory>
class EnemyWaveMgr;
class EnemyWave;
class EnemyMgr;
class EnemyWaveEditor : public Singleton<EnemyWaveEditor>
{
private:
	friend class Singleton<EnemyWaveEditor>;

	bool m_isActive = true;
	bool m_test = false;

	//ï“èWëŒè€
	int m_waveIdx = 0;
	int m_enemyIdx = 0;

	int m_finalWaveTime = 150;

public:
	void EditWithImgui(EnemyWaveMgr& WaveMgr, std::weak_ptr<EnemyMgr> EnemyMgr);
	bool CanWaveUpdate()
	{
		return !m_isActive || m_test;
	}
};