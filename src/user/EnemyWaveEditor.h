//#pragma once
//#include"Singleton.h"
//#include<memory>
//class EnemyWaveMgr;
//class EnemyWave;
//class EnemyMgr;
//class Player;
//class Camera;
//class EnemyWaveEditor : public Singleton<EnemyWaveEditor>
//{
//private:
//	friend class Singleton<EnemyWaveEditor>;
//
//	char* m_fileNameBuff;
//
//	bool m_isActive = false;
//	bool m_test = false;
//
//	//ï“èWëŒè€
//	int m_waveIdx = 0;
//	int m_enemyIdx = 0;
//
//	int m_finalWaveTime = 150;
//
//	bool m_loadMode = false;
//	bool m_saveMode = false;
//
//	EnemyWaveEditor();
//public:
//	void EditWithImgui(EnemyWaveMgr& WaveMgr, std::weak_ptr<EnemyMgr> EnemyMgr, Player& Player);
//	bool CanWaveUpdate()
//	{
//		return !m_isActive || m_test;
//	}
//	void DebugDraw(Camera& Cam, EnemyWaveMgr& WaveMgr);
//};