#include "GrazeParticle.h"
#include"../engine/DrawFuncBillBoard.h"
#include"../engine/Common/KuroFunc.h"
#include"../engine/DrawFunc3D.h"
#include"KazCollisionHelper.h"
#include"../engine/D3D12App.h"

GrazeParticle::GrazeParticle() :SPEED(1000.0f), DISAPPEAR_TIME(120)
{
	texBuff = D3D12App::Instance()->GenerateTextureBuffer("resource/user/Particle/GrazeParticle.png");
	initFlag = false;
	initWallFlag = false;
}

void GrazeParticle::Init(const Vec3<float>& POS, const Vec3<float>& DIR, float SIZE)
{
	m_pos = POS;
	m_vel = DIR;
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
		Vec3<float> mapCentralPos = { 0.0f,0.0f,0.0f };
		Vec3<float> playerPos = m_pos;

		float playerAndMapCenterDistance = playerPos.Distance(mapCentralPos);
		float radius = MAP_SIZE;

		float powNum = pow(radius, 2.0f) - playerAndMapCenterDistance;
		float t = sqrtf(powNum);


		//if (!initWallFlag)
		{
			m_pos += (m_vel * t);
			initWallFlag = true;
		}
		m_pos = KazCollisionHelper::KeepInMap(m_pos, MAP_SIZE);



		//�T�C�Y�k���[�[�[�[�[
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
		//�T�C�Y�k���[�[�[�[�[
	}
}

void GrazeParticle::Draw(Camera& CAMERA)
{
	if (initFlag)
	{
		DrawFuncBillBoard::Graph(CAMERA, m_pos, m_size, texBuff);
	}
}

bool GrazeParticle::IsAlive()
{
	return initFlag;
}