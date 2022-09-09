#pragma once
#include "Vec.h"
#include <vector>
#include <memory>

class EnemyMgr;

namespace ENEMY_INFO {

	enum class ID {

		STOPPING,	// �����Ȃ��G�B
		STRAIGHT,	// �܂����������G�B
		PLAYER_STRAIGHT,	// �����ʒu����v���C���[�̕����ɐ^�����������G�B
		TRACKING,	// �ǔ�����G�B
		SHIELD,		// �������̓G�B
		ID_COUNT,

	};

	struct SPAWN_INFO
	{

		Vec3<float> m_pos;
		Vec3<float> m_forwardVec;
		ID m_id;
		int m_generateFrame;

		SPAWN_INFO() :m_pos(Vec3<float>()), m_forwardVec(Vec3<float>(0, 0, 1)), m_id(ID::STOPPING), m_generateFrame(0) {};

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
	void Update(std::weak_ptr<EnemyMgr> EnemyMgr, const int& NowFrame, const Vec3<float>& PlayerPos, const float& MapSize);
	void AddEnemy(const ENEMY_INFO::SPAWN_INFO& EnemyInfo);
	void AddEnemy(const Vec3<float>& Pos, const Vec3<float>& ForwardVec, ENEMY_INFO::ID ID, const int& GenerateFrame);

};