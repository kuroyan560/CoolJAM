#include "BaseEnemy.h"

void BaseEnemy::Damage(const int& Amount)
{

	/*===== �_���[�W���������� =====*/

	m_hp -= Amount;
	if (m_hp <= 0) {

		Init();

	}

}
