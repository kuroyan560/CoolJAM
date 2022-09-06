#pragma once
#include"../engine/Common/Vec.h"
#include"../user/GrazeParticle.h"
#include<memory>
#include<array>
class Camera;
class GrazeEmitter
{
public:
	GrazeEmitter();
	void Init(const Vec3<float> *POS);
	void Update();
	void Draw(Camera& CAMERA);

private:
	static const int ARRAY_MAX = 50;

	const Vec3<float>*m_posPtr;
	std::array<GrazeParticle, ARRAY_MAX> m_particleArray;
};

