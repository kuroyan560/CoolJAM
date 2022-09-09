#pragma once
#include "Vec.h"
#include <vector>
#include <memory>

class EnemyWave;
class EnemyMgr;

class EnemyWaveMgr {

private:

	/*===== �����o�ϐ� =====*/

	std::vector<std::shared_ptr<EnemyWave>> m_waves;	// �E�F�[�u
	int m_frameTimer;									// �t���[���̃^�C�}�[


public:

	/*===== �����o�֐� =====*/

	EnemyWaveMgr(const float& MapSize);
	void Init();
	void Update(std::weak_ptr<EnemyMgr> EnemyMgr, const Vec3<float>& PlayerPos, const float& MapSize);
	void AddWave(std::shared_ptr<EnemyWave> Wave);

};