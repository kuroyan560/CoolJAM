#pragma once
#include"Vec.h"
#include<memory>
#include<array>
#include<DirectXMath.h>
#include"../engine/Object.h"
#include"../engine/Common/Transform.h"

class Camera;
class DashLightingParticle
{
public:
	DashLightingParticle(std::shared_ptr<ModelObject> MODEL);
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
	std::shared_ptr<Model>m_model;
	Transform m_transform;
};

