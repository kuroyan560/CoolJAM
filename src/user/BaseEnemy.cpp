#include "BaseEnemy.h"
#include "BulletMgr.h"
#include"AudioApp.h"
#include"KuroMath.h"

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
	if (m_appearTimer)
	{
		int timer = APPEAR_TIME - m_appearTimer;

		auto pos = KuroMath::Ease(Out, Quint, timer, APPEAR_TIME,
			m_generatePos + Vec3<float>(0.0f, APPEAR_HEIGHT_OFFSET, 0.0f), m_generatePos);
		m_transform.SetPos(pos);

		Angle radian = KuroMath::Ease(Out, Circ, timer, APPEAR_TIME, Angle::ROUND() * 3.0f, 0.0f);
		m_transform.SetFront(KuroMath::TransformVec3(m_generateForwardVec, KuroMath::RotateMat(Vec3<float>(0, 1, 0), radian)));

		m_appearTimer--;
	}
	else
	{
		OnUpdate(BulletMgr, PlayerPos, MapSize);
	}
}

void BaseEnemy::Generate(ENEMY_INFO::ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos, const Vec3<float> ForwardVec)
{
	OnGenerate(ID, PlayerPos, Pos, ForwardVec);
	m_appearTimer = APPEAR_TIME;
	m_generatePos = Pos;
	m_generateForwardVec = ForwardVec;
	m_transform.SetPos(m_generatePos + Vec3<float>(0.0f, APPEAR_HEIGHT_OFFSET, 0.0f));
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
