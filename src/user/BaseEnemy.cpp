#include "BaseEnemy.h"

void BaseEnemy::Damage(const int& Amount)
{

	/*===== ダメージをあたえる =====*/

	m_hp -= Amount;
	if (m_hp <= 0) {

		Init();

	}

}
