#pragma once
#include"../engine/Common/Vec.h"

class Camera;
class EnemyDeadParticle
{
public:
	EnemyDeadParticle();
	void Init(const Vec3<float> &POS);
	void Update();
	void Draw(Camera &CAMERA);

private:

};

