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
	void Update(bool ENABLE_FLAG);
	void Draw(Camera &Cam);

private:
	Vec3<float> *m_pos;//�v���C���[�̍��W�|�C���^
	Vec3<float> m_startPos, m_endPos;//�v���C���[�̍��W�|�C���^

	std::array<std::unique_ptr<DashLightingParticle>, 40> m_particleArray;//�d���p�[�e�B�N��
	int m_particleInitNum;
	int m_timer;

	bool m_prevFlag;
	int m_roadIndex;
	std::array<std::unique_ptr<DashLightingRoad>, 5> m_roadArray;			//�������̓�
	std::shared_ptr<ModelObject>m_model;
};
