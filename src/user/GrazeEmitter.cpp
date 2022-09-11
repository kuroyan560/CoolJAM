#include "GrazeEmitter.h"
#include"KazCollisionHelper.h"
#include"../engine/UsersInput.h"

GrazeEmitter::GrazeEmitter()
{
}

void GrazeEmitter::Init(const Vec3<float> *POS, const float *VEL)
{
	m_posPtr = POS;
	m_velPtr = VEL;
	initTimer = 0;
}

void GrazeEmitter::Update(float MAP_SIZE, bool HIT_FLAG)
{
	float centralDir = Angle::ConvertToRadian(90);
	//プレイヤーの真後ろに角度を合わせ、ランダムで放射状に発射されるようにする
	float randAngle = KuroFunc::GetRand(-static_cast<float>(RADIAL_LIMIT), static_cast<float>(RADIAL_LIMIT));
	randAngle = Angle::ConvertToRadian(randAngle);
	float cosRadian = cosf(*m_velPtr + centralDir + randAngle);
	float sinRadian = sinf(*m_velPtr + centralDir + randAngle);
	Vec3<float>dir(cosRadian, 0.0f, -sinRadian);

	if (UsersInput::Instance()->KeyOnTrigger(DIK_SPACE))
	{
		bool debug = false;
	}

	++initTimer;
	Vec3<float>pos = *m_posPtr;
	int count = 0;
	for (int i = 0; i < 100; ++i)
	{
		pos += dir * 2.0f;
		float length = pos.Length();
		if (MAP_SIZE <= length)
		{
			count = i;
			break;
		}
		if (i == 99)
		{
			count = i;
		}
	}
	int countMax = 3;
	float rate = (1.0f - (static_cast<float>(count) / static_cast<float>(countMax)));
	if (0 <= initTimer && count <= countMax)
	{
		initTimer = 0;
		for (int i = 0; i < m_particleArray.size(); ++i)
		{
			if (!m_particleArray[i].IsAlive())
			{
				m_particleArray[i].Init(pos, dir, 10.0f * rate);
				break;
			}
		}
	}

	for (int i = 0; i < m_particleArray.size(); ++i)
	{
		m_particleArray[i].Update(MAP_SIZE);
	}
}

void GrazeEmitter::Draw(Camera &CAMERA)
{
	for (int i = 0; i < m_particleArray.size(); ++i)
	{
		m_particleArray[i].Draw(CAMERA);
	}
}
