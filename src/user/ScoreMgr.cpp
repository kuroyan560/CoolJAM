#include "ScoreMgr.h"

void ScoreMgr::Init()
{

	/*===== 初期化処理 =====*/

	m_prevScore = 0;
	m_score = 0;
	m_addScoreRegister = 0;

}

void ScoreMgr::Update()
{

	/*===== 更新処理 =====*/

	// スコアに加算する量を決める。
	float addScorePoint = 0;

	// 一次保存したスコアが0より上だったら。
	if (0 < m_addScoreRegister) {
		addScorePoint = m_addScoreRegister / 2.0f;
	}

	// スコアを加算する。
	m_prevScore = m_score;
	m_score += addScorePoint;
	m_addScoreRegister -= addScorePoint;

}

void ScoreMgr::Draw()
{

	/*===== 描画処理 =====*/

}

void ScoreMgr::AddScore(const int& Score)
{

	/*===== 一次保存スコアを加算する =====*/

	m_addScoreRegister += Score;

}

void ScoreMgr::GameFinish()
{

	/*===== 保存してあるスコアを一気に代入する =====*/

	m_score = m_addScoreRegister;

}
