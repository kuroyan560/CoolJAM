#include "GrazeParticle.h"
#include"../engine/DrawFuncBillBoard.h"
#include"../engine/Common/KuroFunc.h"
#include"../engine/DrawFunc3D.h"
#include"KazCollisionHelper.h"
#include"../engine/D3D12App.h"

GrazeParticle::GrazeParticle() :SPEED(1000.0f), DISAPPEAR_TIME(60)
{
	texBuff = KuroEngine::Instance();;
}

void GrazeParticle::Init(const Vec3<float>& POS, const Vec3<float>& DIR, float SIZE)
{
	m_pos = POS;
	m_vel = DIR * SPEED;
	m_size = { SIZE ,SIZE };

	float disppaerPerSpeed = SIZE / static_cast<float>(DISAPPEAR_TIME);
	m_dispappearSpeed = { disppaerPerSpeed,disppaerPerSpeed };
}

void GrazeParticle::Update()
{
	m_pos += m_vel;
	m_pos = KazCollisionHelper::KeepInMap(m_pos, 5.0f);


	//サイズ縮小ーーーーー
	m_size -= m_dispappearSpeed;
	if (m_size.x <= 0.0f)
	{
		m_size.x = 0.0f;
	}
	if (m_size.y <= 0.0f)
	{
		m_size.y = 0.0f;
	}
	//サイズ縮小ーーーーー
}

void GrazeParticle::Draw(Camera& CAMERA)
{
	DrawFuncBillBoard::Graph(CAMERA, m_pos, m_size, texBuff);
}
