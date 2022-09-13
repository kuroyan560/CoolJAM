#pragma once
#include "Singleton.h"

class ScoreMgr : public Singleton<ScoreMgr> {

private:

	/*===== メンバ変数 =====*/

	int m_prevScore;			// 1F前のスコア 演出用
	int m_score;			// 現在のスコア
	int m_addScoreRegister;	// スコアに加算する量。スコアを加算する際は一度この値に入れる。スコアががらがら増える演出のため。


public:

	/*===== メンバ関数 =====*/

	void Init();
	void Update();
	void Draw();

	void AddScore(const int& Score);	// スコアを加算する。
	void GameFinish();					// 保存してあるスコアを一気に代入する。

};