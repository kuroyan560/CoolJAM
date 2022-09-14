#include "BaseEnemy.h"
#include "BulletMgr.h"
#include"AudioApp.h"
#include"KuroMath.h"
#include"Importer.h"
#include"KuroEngine.h"

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

		DrawFunc_Append::DrawPlane(transform, s_appearReticleTex[m_appearReticleTexIdx], RenderTargetSwitch(alpha, 0.0f, 1.0f), AlphaBlendMode_Trans);
	}

	OnDraw();
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
