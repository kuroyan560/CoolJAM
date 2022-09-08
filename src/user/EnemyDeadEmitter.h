#pragma once
#include"../user/EnemyDeadLineParticle.h"
#include<array>
#include"../engine/Common/Vec.h"

class Camera;
class EnemyDeadEmitter
{
public:
	EnemyDeadEmitter();
	void Init(const Vec3<float> &POS);
	void Update();
	void Draw(Camera &CAMERA);

private:
	Vec3<float>pos;
	std::array<EnemyDeadLineParticle, 20>m_lineParticle;

};

