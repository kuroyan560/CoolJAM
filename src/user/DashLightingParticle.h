#pragma once
#include"Vec.h"
#include<memory>
#include<array>
#include<DirectXMath.h>
#include"../engine/Object.h"

class Camera;
class DashLightingParticle
{
public:
	DashLightingParticle();
	void Init(const Vec3<float> &POS);
	void Update();
	void Draw(Camera &CAMEARA);

private:
	Vec3<float> m_pos;
	Vec3<float> m_startPos, m_endPos;
	float angle;
	float angleVel;
	int alpha;
	bool initFlag;
	bool firstDrawFlag;
	std::shared_ptr<ModelObject>m_model;
};

