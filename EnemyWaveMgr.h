#pragma once
#include "Vec.h"
#include <vector>
#include <memory>

class EnemyWave;

class EnemyWaveMgr {

private:

	/*===== �����o�ϐ� =====*/

	std::vector<std::shared_ptr<EnemyWave>> m_waves;	// �E�F�[�u
	int m_frameTimer;									// �t���[���̃^�C�}�[


public:

	/*===== �����o�֐� =====*/

	EnemyWaveMgr();
	void Init();
	void Update(const Vec3<float>& PlayerPos);

};