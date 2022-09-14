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
	void Init(const Vec3<float> *POS,const float *VEL);
	void Update(float MAP_SIZE);
	void Draw(Camera& CAMERA);

private:
	static const int ARRAY_MAX = 300;
	static const int RADIAL_LIMIT = 120;

	const Vec3<float>*m_posPtr;
	const float *m_velPtr;
	std::array<GrazeParticle, ARRAY_MAX> m_particleArray;
	float initTimer;

};

