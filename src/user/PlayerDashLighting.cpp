#include "PlayerDashLighting.h"
#include"../engine/DrawFunc3D.h"

PlayerDashLighting::PlayerDashLighting()
{
}

void PlayerDashLighting::Init(Vec3<float> *POS)
{
	pos = POS;
	timer = 0;
}

void PlayerDashLighting::Update(bool ENABLE_FLAG)
{
	++timer;
	if (2 <= timer && ENABLE_FLAG)
	{
		Vec3<float>emittPos;
		emittPos.x = pos->x + KuroFunc::GetRand(-3.0f, 3.0f);
		emittPos.y = KuroFunc::GetRand(-3.0f, 3.0f);
		emittPos.z = pos->z + KuroFunc::GetRand(-3.0f, 3.0f);
		particleArray[particleInitNum].Init(emittPos);

		++particleInitNum;
		timer = 0;
	}
	if (particleArray.size() <= particleInitNum)
	{
		particleInitNum = 0;
	}

	for (int i = 0; i < particleArray.size(); ++i)
	{
		particleArray[i].Update();
	}

	if (ENABLE_FLAG && !prevFlag)
	{
		startPos = *pos;
	}
	if(ENABLE_FLAG)
	{
		endPos = *pos;
	}
	prevFlag = ENABLE_FLAG;
}

void PlayerDashLighting::Draw(Camera &CAMERA)
{
	for (int i = 0; i < particleArray.size(); ++i)
	{
		particleArray[i].Draw(CAMERA);
	}

	DrawFunc3D::DrawLine(CAMERA, startPos, endPos, Color(255, 0, 0, 255), 1.0f);
}
