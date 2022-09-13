#include "BaseEnemy.h"
#include "BulletMgr.h"
#include"AudioApp.h"

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

void BaseEnemy::Update(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize)
{
	OnUpdate(BulletMgr, PlayerPos, MapSize);
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

		Init();

		AudioApp::Instance()->PlayWaveDelay(s_deadSE);
	}
	else
	{
		AudioApp::Instance()->PlayWaveDelay(s_damageSE);
	}

}
