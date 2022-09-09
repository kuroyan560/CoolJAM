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
	void Update(bool ENABLE_FLAG);
	void Draw(Camera &Cam);

private:
	Vec3<float> *pos;
	std::array<DashLightingParticle, 40> particleArray;
	int particleInitNum;
	int timer;


	Vec3<float> startPos, endPos;
	bool prevFlag;
};
