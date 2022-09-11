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
	Vec3<float> m_startPos, m_endPos;

	static int s_dashLightIndex;
	int m_indexNum;
	Color m_color;
	bool m_initFlag;
};

