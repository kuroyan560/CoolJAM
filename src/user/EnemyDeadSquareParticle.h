#pragma once
#include"../engine/Common/Vec.h"
#include "Importer.h"
#include "Model.h"
#include "Transform.h"
#include"D3D12Data.h"

class Camera;
class EnemyDeadSquareParticle
{
public:
	static const std::vector<InputLayoutParam>& GetInputLayout()
	{
		static std::vector<InputLayoutParam>layout =
		{
			InputLayoutParam("ANGLE",DXGI_FORMAT_R32G32B32_FLOAT),
			InputLayoutParam("INIT_FLG",DXGI_FORMAT_R32_SINT),
			InputLayoutParam("ANGLE_VEL",DXGI_FORMAT_R32G32B32_FLOAT),
			InputLayoutParam("ALPHA",DXGI_FORMAT_R32_SINT),
			InputLayoutParam("POS",DXGI_FORMAT_R32G32B32_FLOAT),
			InputLayoutParam("DISAPPEAR_TIME",DXGI_FORMAT_R32_SINT),
			InputLayoutParam("VEL",DXGI_FORMAT_R32G32B32_FLOAT),
		};
		return layout;
	}

	EnemyDeadSquareParticle();
	void Init(const Vec3<float> &POS, float SPEED, int ANGLE, const Color &COLOR);
	void Update();
	void Draw(Camera &CAMERA);

private:
	Vec3<Angle> m_angle;
	int m_initFlag = 0;
	Vec3<Angle> m_angleVel;
	int m_alpha;
	Vec3<float> m_pos;
	int m_dispappearTime;
	Vec3<float>m_vel;
	float pad;
};

