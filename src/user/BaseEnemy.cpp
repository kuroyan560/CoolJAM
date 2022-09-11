#include "BaseEnemy.h"
#include "BulletMgr.h"

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

	}

}
