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
	Vec3<float> *pos;//�v���C���[�̍��W�|�C���^
	Vec3<float> startPos, endPos;//�v���C���[�̍��W�|�C���^

	std::array<DashLightingParticle, 40> particleArray;//�d���p�[�e�B�N��
	int particleInitNum;
	int timer;

	bool prevFlag;
	int roadIndex;
	std::array<DashLightingRoad, 5> roadArray;			//�������̓�
};
