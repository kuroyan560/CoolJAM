#include "BaseEnemy.h"
#include "BulletMgr.h"

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

	}

}
