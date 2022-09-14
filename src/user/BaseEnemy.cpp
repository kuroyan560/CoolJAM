#include "BaseEnemy.h"
#include "BulletMgr.h"
#include"AudioApp.h"
#include"ScoreMgr.h"

int BaseEnemy::s_deadSE = -1;
int BaseEnemy::s_damageSE = -1;

BaseEnemy::BaseEnemy()
{
	if (s_deadSE == -1)
	{
		s_deadSE = AudioApp::Instance()->LoadAudio("resource/user/sound/enemyDead.wav");
		s_damageSE = AudioApp::Instance()->LoadAudio("resource/user/sound/hit.wav", 0.1f);
	}
}

void BaseEnemy::Damage(const int& Amount, std::weak_ptr<BulletMgr> BulletMgr)
{

	/*===== ダメージをあたえる =====*/

	m_hp -= Amount;
	if (m_hp <= 0) {

		if (m_id == ENEMY_INFO::ID::ELEC_MUSHI) {
			// エレキ虫が死んだ。
			BulletMgr.lock()->KillElecMushi();
		}


		switch (m_id)
		{
		case ENEMY_INFO::ID::STOPPING:
		case ENEMY_INFO::ID::STRAIGHT:
		case ENEMY_INFO::ID::PLAYER_STRAIGHT:
		case ENEMY_INFO::ID::TORUS_MOVE:

			ScoreMgr::Instance()->AddScore(MIDDLE_SCORE_POINT);

			break;

		case ENEMY_INFO::ID::TRACKING:

			ScoreMgr::Instance()->AddScore(LOW_SCORE_POINT);

			break;

		case ENEMY_INFO::ID::SHIELD:
		case ENEMY_INFO::ID::UNION:
		case ENEMY_INFO::ID::PRESS:
		case ENEMY_INFO::ID::ELEC_MUSHI:

			ScoreMgr::Instance()->AddScore(HIGHT_SCORE_POOINT);

			break;
		default:
			break;
		}



		Init();

		AudioApp::Instance()->PlayWaveDelay(s_deadSE);
	}
	else
	{
		AudioApp::Instance()->PlayWaveDelay(s_damageSE);
	}

}
