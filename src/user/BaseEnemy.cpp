#include "BaseEnemy.h"
#include "BulletMgr.h"
#include"AudioApp.h"
#include"KuroMath.h"
#include"Importer.h"
#include"KuroEngine.h"
#include"ScoreMgr.h"

int BaseEnemy::s_deadSE = -1;
int BaseEnemy::s_damageSE = -1;
std::shared_ptr<Model>BaseEnemy::s_appearReticleModel;
std::array<std::shared_ptr<TextureBuffer>, 5>BaseEnemy::s_appearReticleTex;

BaseEnemy::BaseEnemy()
{
	if (s_deadSE == -1)
	{
		s_deadSE = AudioApp::Instance()->LoadAudio("resource/user/sound/enemyDead.wav");
		s_damageSE = AudioApp::Instance()->LoadAudio("resource/user/sound/hit.wav", 0.1f);

		s_appearReticleModel = Importer::Instance()->LoadModel("resource/user/", "enemy_appearReticle.glb");
		D3D12App::Instance()->GenerateTextureBuffer(s_appearReticleTex.data(), "resource/user/appearReticle.png", 5, { 5,1 });
	}
}

void BaseEnemy::Init()
{
	OnInit();
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

		m_appearReticleAngle += Angle::PI() / 180.0f;

		m_appearTimer--;

		if (m_appearTimer % RETICLE_TEX_CHANGE_SPAN == 0)
		{
			m_appearReticleTexIdx++;

			if (5 <= m_appearReticleTexIdx)m_appearReticleTexIdx = 0;
		}
	}
	else if (m_disappearTimer)
	{
		int timer = DISAPPEAR_TIME - m_disappearTimer;

		m_disappearTimer--;

		auto scale = KuroMath::Ease(In, Back, timer, DISAPPEAR_TIME, m_disAppearStartScale, { 0,0,0 });
		m_transform.SetScale(scale);

		auto angle = KuroMath::Ease(In, Back, timer, DISAPPEAR_TIME, 0.0f, Angle(DISAPPEAR_SPIN_DEGREE));
		m_transform.SetFront(KuroMath::TransformVec3(m_disappearStartFront, KuroMath::RotateMat(Vec3<float>(0, 1, 0), angle)));

		auto pos = m_transform.GetPos();
		pos.y += KuroMath::Ease(In, Circ, timer, DISAPPEAR_TIME, 0.0f, DISAPPEAR_HEIGHT_OFFSET);
		m_transform.SetPos(pos);

		if (m_disappearTimer <= 0)Init();
	}

	else
	{
		OnUpdate(BulletMgr, PlayerPos, MapSize);
	}
}

void BaseEnemy::Generate(ENEMY_INFO::ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos, const Vec3<float> ForwardVec)
{
	OnGenerate(ID, PlayerPos, Pos, ForwardVec);
	m_appearReticleTexIdx = 0;
	m_appearTimer = APPEAR_TIME;
	m_appearReticleHeight = KuroFunc::GetRand(1.0f, 3.0f);
	m_generatePos = Pos;
	m_generateForwardVec = ForwardVec;
	m_transform.SetPos(m_generatePos + Vec3<float>(0.0f, APPEAR_HEIGHT_OFFSET, 0.0f));

	m_disappearTimer = 0;
}

#include"DrawFunc_Append.h"
void BaseEnemy::Draw()
{
	//登場位置にレティクル描画
	if (m_appearTimer)
	{
		Transform transform;

		auto reticlePos = m_transform.GetPos();
		reticlePos.y = m_appearReticleHeight;
		transform.SetPos(reticlePos);

		int timer = APPEAR_TIME - m_appearTimer;
		float scale = KuroMath::Ease(Out, Exp, timer, APPEAR_TIME,
			RETICLE_SCALE_MAX, RETICLE_SCALE_MIN);
		transform.SetScale({ scale,1.0f,scale });

		transform.SetRotate(Vec3<float>(0, 1, 0), m_appearReticleAngle);

		float alpha = KuroMath::Ease(In, Circ, timer, APPEAR_TIME, 1.0f, 0.0f);

		DrawFunc_Append::DrawPlane(transform, s_appearReticleTex[m_appearReticleTexIdx], RenderTargetSwitch(alpha, 1.0f, 1.0f), AlphaBlendMode_Trans);
	}

	OnDraw();
}

void BaseEnemy::Disappear()
{
	m_disappearTimer = DISAPPEAR_TIME;
	m_disAppearStartScale = m_transform.GetScale();
	m_disappearStartFront = m_transform.GetFront();
	m_disappearStartHeight = m_transform.GetPos().y;
	m_disappear = true;
}

void BaseEnemy::Damage(const int& Amount, std::weak_ptr<BulletMgr> BulletMgr)
{

	/*===== ダメージをあたえる =====*/

	m_hp -= Amount;
	if (m_hp <= 0) {

		m_disappear = false;

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
