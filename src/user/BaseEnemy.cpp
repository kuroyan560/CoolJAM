#include "BaseEnemy.h"

void BaseEnemy::Damage(const int& Amount)
{

	/*===== ƒ_ƒ[ƒW‚ğ‚ ‚½‚¦‚é =====*/

	m_hp -= Amount;
	if (m_hp <= 0) {

		Init();

	}

}
