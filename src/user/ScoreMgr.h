#pragma once
#include "Singleton.h"

class ScoreMgr : public Singleton<ScoreMgr> {

private:

	/*===== �����o�ϐ� =====*/

	int m_prevScore;			// 1F�O�̃X�R�A ���o�p
	int m_score;			// ���݂̃X�R�A
	int m_addScoreRegister;	// �X�R�A�ɉ��Z����ʁB�X�R�A�����Z����ۂ͈�x���̒l�ɓ����B�X�R�A�����炪�瑝���鉉�o�̂��߁B


public:

	/*===== �����o�֐� =====*/

	void Init();
	void Update();
	void Draw();

	void AddScore(const int& Score);	// �X�R�A�����Z����B
	void GameFinish();					// �ۑ����Ă���X�R�A����C�ɑ������B

};