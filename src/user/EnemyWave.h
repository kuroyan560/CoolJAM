#pragma once
#include "Vec.h"
#include <vector>

namespace ENEMY_INFO {

	enum class ID {

		STOPPING_ENEMY,	// 動かない敵。
		TRACKING_ENEMY,	// 追尾する敵。
		STRAIGHT_ENEMY,	// まっすぐ動く敵。
		PLAYER_STRAIGHT_ENEMY,	// 生成位置からプレイヤーの方向に真っ直ぐ動く敵。
		ID_COUNT,

	};

	struct SPAWN_INFO
	{

		Vec3<float> m_pos;	

	};


}

class EnemyWave {

private:

	/*===== メンバ変数 =====*/

	std::vector<ENEMY_INFO::SPAWN_INFO> m_enemys;	// 敵データ
	int m_waveStartFrame;							// ウェーブ開始フレーム


public:

	/*===== メンバ関数 =====*/

	EnemyWave(const int& WaveStartFrame);
	void Update(const int& NowFrame, const Vec3<float>& PlayerPos);

};