#pragma once
#include"Vec.h"
#include<memory>
#include<array>
#include<DirectXMath.h>
#include"DashLightingParticle.h"
#include"DashLightingRoad.h"

class Camera;
class PlayerDashLighting
{
public:
	PlayerDashLighting();
	void Init(Vec3<float> *POS);
	void Update(bool ENABLE_FLAG);
	void Draw(Camera &Cam);

private:
	Vec3<float> *pos;//プレイヤーの座標ポインタ
	Vec3<float> startPos, endPos;//プレイヤーの座標ポインタ

	std::array<DashLightingParticle, 40> particleArray;//電撃パーティクル
	int particleInitNum;
	int timer;

	bool prevFlag;
	int roadIndex;
	std::array<DashLightingRoad, 5> roadArray;			//加速時の道
};
