#include "CoinEnemy.h"
#include"DrawFunc_Append.h"

CoinEnemy::CoinEnemy(std::shared_ptr<Model> DefModel, std::shared_ptr<Model> DamageModel)
{
	m_model = DefModel;
	m_modelHit = DamageModel;
	m_isActive = false;
}

void CoinEnemy::Init()
{
	m_isActive = false;
}

void CoinEnemy::Generate(ENEMY_INFO::ID ID, const Vec3<float> &PlayerPos, const Vec3<float> &Pos, const Vec3<float> ForwardVec)
{
	m_id = ID;
	m_pos = Pos;
	m_isActive = true;
	m_scale = 3.0f;
}

void CoinEnemy::Update(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float> &PlayerPos, const float &MapSize)
{
	m_transform.SetPos(m_pos);
	m_transform.SetScale(m_scale);
}

void CoinEnemy::Draw()
{
	DrawFunc_Append::DrawModel(m_model, m_transform);
}