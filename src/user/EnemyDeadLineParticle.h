#pragma once
#include"../engine/Common/Vec.h"
#include"D3D12Data.h"

class Camera;
class EnemyDeadLineParticle
{
public:
	static const std::vector<InputLayoutParam>&GetInputLayout()
	{
		static std::vector<InputLayoutParam>layout =
		{
			InputLayoutParam("POS",DXGI_FORMAT_R32G32B32_FLOAT),
			InputLayoutParam("INIT_FLG",DXGI_FORMAT_R32_SINT),
			InputLayoutParam("VEL",DXGI_FORMAT_R32G32B32_FLOAT),
			InputLayoutParam("LEN",DXGI_FORMAT_R32_FLOAT),
			InputLayoutParam("START_POS",DXGI_FORMAT_R32G32B32_FLOAT),
			InputLayoutParam("ALPHA",DXGI_FORMAT_R32_SINT),
			InputLayoutParam("END_POS",DXGI_FORMAT_R32G32B32_FLOAT),
			InputLayoutParam("DISAPPEAR_TIME",DXGI_FORMAT_R32_SINT),
			InputLayoutParam("COLOR",DXGI_FORMAT_R32G32B32_SINT),
		};
		return layout;
	}

	EnemyDeadLineParticle();
	void Init(const Vec3<float> &POS, float SPEED, int ANGLE,const Color &COLOR);
	void Update();
	void Draw(Camera &CAMERA);

private:
	Vec3<float> m_pos;
	int m_initFlag;
	Vec3<float>m_vel;
	float m_length;
	Vec3<float> m_startPos;
	int m_alpha;
	Vec3<float>m_endPos;
	int m_dispappearTime;
	Vec3<int>m_rgb;
};