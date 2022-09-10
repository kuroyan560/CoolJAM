#pragma once
#include"Vec.h"
#include<memory>
#include<array>
#include<DirectXMath.h>
#include"DashLightingParticle.h"
#include"../engine/Common/Color.h"

class DashLightingRoad
{
public:
	DashLightingRoad();
	void Init(const Vec3<float> &POS);
	void Update(const Vec3<float> &POS, int NOW_INDEX);
	void Draw(Camera &Cam);

private:
	Vec3<float> startPos, endPos;

	static int INDEX;
	int indexNum;
	Color color;
	bool initFlag;
};

