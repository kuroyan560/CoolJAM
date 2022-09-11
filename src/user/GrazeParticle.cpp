#include "GrazeParticle.h"
#include"../engine/DrawFuncBillBoard.h"
#include"../engine/Common/KuroFunc.h"
#include"../engine/DrawFunc3D.h"
#include"KazCollisionHelper.h"
#include"../engine/D3D12App.h"
#include"KazDrawFunc.h"
#include"../user/DrawFunc_Append.h"

GrazeParticle::GrazeParticle() :SPEED(1000.0f), DISAPPEAR_TIME(60)
{
	texBuff = D3D12App::Instance()->GenerateTextureBuffer("resource/user/Particle/GrazeParticle.png");
	initFlag = false;
	initWallFlag = false;
}

void GrazeParticle::Init(const Vec3<float>& POS, const Vec3<float>& DIR, float SIZE)
{
	m_pos = POS;
	m_vel = DIR;

	Vec3<float>pos = m_pos;
	for (int i = 0; i < 100; ++i)
	{
		pos += m_vel * 2.0f;
		if (100.0f <= pos.Length())
		{
			break;
		}
	}
	float speed = m_pos.Distance(pos);;
	m_vel *= speed;
	m_pos = pos;
	m_size = { SIZE ,SIZE };

	float disppaerPerSpeed = SIZE / static_cast<float>(DISAPPEAR_TIME);
	m_dispappearSpeed = { disppaerPerSpeed,disppaerPerSpeed };
	initFlag = true;
	initWallFlag = false;
}

void GrazeParticle::Update(float MAP_SIZE)
{
	if (initFlag)
	{
		m_pos = KazCollisionHelper::KeepInMap(m_pos, MAP_SIZE);

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
		if (m_size.x <= 0.0f && m_size.y <= 0.0f)
		{
			initFlag = false;
		}
		//サイズ縮小ーーーーー
	}
}

void GrazeParticle::Draw(Camera& CAMERA)
{
	if (initFlag)
	{
		KazDrawFunc::DrawBillBoardGraph(CAMERA, m_pos, m_size, texBuff);
	}
}

bool GrazeParticle::IsAlive()
{
	return initFlag;
}