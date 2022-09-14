#pragma once
#include"Singleton.h"
#include<memory>
#include"../engine/Common/Vec.h"
#include"../engine/Common/Angle.h"
class ModelObject;
class RenderTarget;

class StageFloor : public Singleton<StageFloor>
{
	friend class Singleton<StageFloor>;

	std::shared_ptr<ModelObject>m_modelObj;
	std::shared_ptr<RenderTarget>m_floorScreen;
	Vec3<Angle>m_rotate;
	StageFloor();
public:
	void ClearScreen();
	void ScreenTargetSet(bool Clear);
	void Draw();
};