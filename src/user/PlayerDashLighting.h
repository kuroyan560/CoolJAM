#pragma once
#include"Vec.h"
#include<memory>
#include<array>
#include<DirectXMath.h>
#include"DashLightingParticle.h"

class Camera;
class PlayerDashLighting
{
public:
	PlayerDashLighting();
	void Init(Vec3<float> *POS);
	void Update();
	void Draw(Camera &Cam);

private:
	Vec3<float> *pos;
	Vec3<float> startPos, endPos;

	std::array<DashLightingParticle, 20> particleArray;
	int particleInitNum;
	int timer;
};
