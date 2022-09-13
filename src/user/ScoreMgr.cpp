#include "ScoreMgr.h"

void ScoreMgr::Init()
{

	/*===== ���������� =====*/

	m_prevScore = 0;
	m_score = 0;
	m_addScoreRegister = 0;

}

void ScoreMgr::Update()
{

	/*===== �X�V���� =====*/

	// �X�R�A�ɉ��Z����ʂ����߂�B
	float addScorePoint = 0;

	// �ꎟ�ۑ������X�R�A��0���ゾ������B
	if (0 < m_addScoreRegister) {
		addScorePoint = m_addScoreRegister / 2.0f;
	}

	// �X�R�A�����Z����B
	m_prevScore = m_score;
	m_score += addScorePoint;
	m_addScoreRegister -= addScorePoint;

}

void ScoreMgr::Draw()
{

	/*===== �`�揈�� =====*/

}

void ScoreMgr::AddScore(const int& Score)
{

	/*===== �ꎟ�ۑ��X�R�A�����Z���� =====*/

	m_addScoreRegister += Score;

}

void ScoreMgr::GameFinish()
{

	/*===== �ۑ����Ă���X�R�A����C�ɑ������ =====*/

	m_score = m_addScoreRegister;

}
