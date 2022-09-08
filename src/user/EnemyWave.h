#pragma once
#include "Vec.h"
#include <vector>

namespace ENEMY_INFO {

	enum class ID {

		STOPPING_ENEMY,	// �����Ȃ��G�B
		TRACKING_ENEMY,	// �ǔ�����G�B
		STRAIGHT_ENEMY,	// �܂����������G�B
		PLAYER_STRAIGHT_ENEMY,	// �����ʒu����v���C���[�̕����ɐ^�����������G�B
		ID_COUNT,

	};

	struct SPAWN_INFO
	{

		Vec3<float> m_pos;	

	};


}

class EnemyWave {

private:

	/*===== �����o�ϐ� =====*/

	std::vector<ENEMY_INFO::SPAWN_INFO> m_enemys;	// �G�f�[�^
	int m_waveStartFrame;							// �E�F�[�u�J�n�t���[��


public:

	/*===== �����o�֐� =====*/

	EnemyWave(const int& WaveStartFrame);
	void Update(const int& NowFrame, const Vec3<float>& PlayerPos);

};