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

void BaseEnemy::Damage(const int& Amount, std::weak_ptr<BulletMgr> BulletMgr)
{

	/*===== �_���[�W���������� =====*/

	m_hp -= Amount;
	if (m_hp <= 0) {

		if (m_id == ENEMY_INFO::ID::ELEC_MUSHI) {
			// �G���L�������񂾁B
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
