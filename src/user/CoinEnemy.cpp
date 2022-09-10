#include "CoinEnemy.h"
#include"DrawFunc3D.h"

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

	baseEnemy_id = ID;
}

void CoinEnemy::Update(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float> &PlayerPos, const float &MapSize)
{
	m_transform.SetPos(m_pos);
	m_transform.SetScale(m_scale);
}

void CoinEnemy::Draw(Camera &Cam)
{
	DrawFunc3D::DrawNonShadingModel(m_modelHit, m_transform, Cam);
}

bool CoinEnemy::GetIsActive()
{
	return m_isActive;
}

float CoinEnemy::GetScale()
{
	return m_scale;
}

Vec3<float> CoinEnemy::GetPos()
{
	return m_pos;
}
