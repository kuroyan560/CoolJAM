#pragma once
#include"../engine/Common/Vec.h"
#include "Importer.h"
#include "Model.h"
#include "Transform.h"

class Camera;
class EnemyDeadSquareParticle
{
public:
	EnemyDeadSquareParticle(std::shared_ptr<Model>MODEL);
	void Init(const Vec3<float> &POS, float SPEED, int ANGLE);
	void Update();
	void Draw(Camera &CAMERA);

private:
	std::shared_ptr<Model> m_model;
	Transform m_transform;
	Vec3<Angle> m_angle;
	Vec3<Angle> m_angleVel;

	Vec3<float> m_pos, m_vel;
	bool m_initFlag;
	int m_alpha;
	int m_dispappearTime;
	int timer;
	float sinVel;
};

