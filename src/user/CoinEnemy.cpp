#include "CoinEnemy.h"
#include"DrawFunc_Append.h"

CoinEnemy::CoinEnemy(std::shared_ptr<Model> DefModel, std::shared_ptr<Model> DamageModel)
{
	m_model = DefModel;
	m_modelHit = DamageModel;
	m_isActive = false;
}

void CoinEnemy::OnInit()
{
	m_isActive = false;
	m_angle = Angle(90);
}

void CoinEnemy::OnGenerate(ENEMY_INFO::ID ID, const Vec3<float> &PlayerPos, const Vec3<float> &Pos, const Vec3<float> ForwardVec)
{
	m_id = ID;
	m_pos = Pos;
	m_isActive = true;
	m_scale = 5.0f;
}

void CoinEnemy::OnUpdate(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float> &PlayerPos, const float &MapSize)
{
	m_transform.SetPos(m_pos);
	m_angle += Angle::ConvertToRadian(1.0f);
}

void CoinEnemy::OnDraw()
{
	m_transform.SetRotate(Vec3<float>(0, 1, 0), m_angle);
	m_transform.SetScale(m_scale);
	DrawFunc_Append::DrawModel(m_model, m_transform);
}