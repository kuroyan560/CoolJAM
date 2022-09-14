#pragma once
#include "Vec.h"
#include <vector>
#include <memory>
#include<string>
#include<array>

class EnemyMgr;

namespace ENEMY_INFO {

	enum class ID {

		STOPPING,		// �����Ȃ��G�B
		STRAIGHT,		// �܂����������G�B
		PLAYER_STRAIGHT,// �����ʒu����v���C���[�̕����ɐ^�����������G�B
		TRACKING,		// �ǔ�����G�B
		SHIELD,			// �������̓G�B
		UNION,			// �W���̂̓G�B
		TORUS_MOVE,		// ���S�����悤�ɓ����B
		PRESS,			// �����߂��G�B
		DASH,			// �_�b�V������G�B
		ELEC_MUSHI,		// �G���L��
		COIN,			// �R�C��
		ID_COUNT,

	};

	static const std::string& GetIDName(ID Id)
	{
		static const std::array<std::string, static_cast<int>(ID::ID_COUNT)> NAME_LIST =
		{
			"STOPPING",
			"STRAIGHT",
			"PLAYER_STRAIGHT",
			"TRACKING",
			"SHIELD",
			"UNION",
			"TORUS_MOVE",
			"PRESS",
			"DASH",
			"ELEC_MUSI",
			"COIN"
		};
		return NAME_LIST[static_cast<int>(Id)];
	}

	struct SPAWN_INFO
	{

		Vec3<float> m_pos;
		Vec3<float> m_forwardVec;
		ID m_id;
		int m_generateFrame;
		int m_shotTimer;

		SPAWN_INFO() :m_pos(Vec3<float>()), m_forwardVec(Vec3<float>(0, 0, 1)), m_id(ID::STOPPING), m_generateFrame(0), m_shotTimer(0) {};

	};


}

class EnemyWave {

private:
	friend class EnemyWaveEditor;
	friend class EnemyWaveLoader;

	/*===== �����o�ϐ� =====*/

	std::vector<ENEMY_INFO::SPAWN_INFO> m_enemys;	// �G�f�[�^
	int m_waveStartFrame;							// �E�F�[�u�J�n�t���[��
	float m_nowWaveFrame;							// ����Wave���n�܂��Ă���̌o�ߎ��ԁB
	bool m_isBounusStageFlag;
	bool m_startWaveFlag;

	//�G�̐�����
	int m_generateCount;
public:

	/*===== �����o�֐� =====*/

	EnemyWave(const int& WaveStartFrame, const bool& BounusStageFlag);
	void Init();
	void Update(std::weak_ptr<EnemyMgr> EnemyMgr, const Vec3<float>& PlayerPos, const float& MapSize);
	void AddEnemy(const ENEMY_INFO::SPAWN_INFO& EnemyInfo);
	void AddEnemy(const Vec3<float>& Pos, const Vec3<float>& ForwardVec, ENEMY_INFO::ID ID, const int& GenerateFrame, const int& ShotTimer);

	void Start();
	void Stop();
	bool IsBounusStage();
	int WaveStartTime();

	bool IsAllEnemyAppear()const { return m_generateCount == static_cast<int>(m_enemys.size()); }

	int GetWaveStartFrame() { return m_waveStartFrame; }

	// ���̃E�F�[�u�ōŌ�ɓG���N�����[�J���̃t���[���������Ƃ߂�B�@���[�J���̃t���[�� = EnemyWave���̃t���[��
	int GetWaveEndFrameLocal();

};

