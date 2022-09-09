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

void PlayerDashLighting::Update()
{
	++timer;
	if (5 <= timer)
	{
		particleArray[particleInitNum].Init(*pos);
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
}

void PlayerDashLighting::Draw(Camera &CAMERA)
{
	for (int i = 0; i < particleArray.size(); ++i)
	{
		particleArray[i].Draw(CAMERA);
	}
}
