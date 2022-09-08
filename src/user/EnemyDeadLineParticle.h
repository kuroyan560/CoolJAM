#pragma once
#include"../engine/Common/Vec.h"

class Camera;
class EnemyDeadLineParticle
{
public:
	EnemyDeadLineParticle();
	void Init(const Vec3<float> &POS, int ANGLE);
	void Update();
	void Draw(Camera &CAMERA);

private:
	Vec3<float> m_pos, m_vel;
	Vec3<float> m_startPos, m_endPos;
	bool m_initFlag;
	float m_length;
	int m_alpha;
	int m_dispappearTime;
};