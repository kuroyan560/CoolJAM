#pragma once
#include"Vec.h"
#include<memory>
#include<array>
#include<DirectXMath.h>
#include"DashLightingParticle.h"
#include"DashLightingRoad.h"

class Camera;
class PlayerDashLighting
{
public:
	PlayerDashLighting();
	void Init(Vec3<float> *POS);
	void Update(bool ENABLE_FLAG, bool FEVER_FLAG);
	void Draw(Camera &Cam);

private:
	Vec3<float> *m_pos;//プレイヤーの座標ポインタ
	Vec3<float> m_startPos, m_endPos;//プレイヤーの座標ポインタ

	std::array<std::unique_ptr<DashLightingParticle>, 40> m_particleArray;//電撃パーティクル
	int m_particleInitNum;
	float m_timer;

	bool m_prevFlag;
	int m_roadIndex;
	std::array<std::unique_ptr<DashLightingRoad>, 5> m_roadArray;			//加速時の道
	std::shared_ptr<ModelObject>m_model;

	std::array<std::shared_ptr<TextureBuffer>, 3>elecAnimationTexture;

	std::shared_ptr<Model>m_lModel;
};
