#pragma once
#include"Singleton.h"
#include<memory>
#include"../engine/Common/Vec.h"
#include"../engine/Common/Angle.h"
#include"Transform.h"
class Model;
class RenderTarget;

class StageFloor : public Singleton<StageFloor>
{
	friend class Singleton<StageFloor>;

	std::shared_ptr<Model>m_floorModel;
	std::shared_ptr<Model>m_circleModel;
	Transform m_transform;
	std::shared_ptr<RenderTarget>m_floorScreen;
	Vec3<Angle>m_rotate;
	StageFloor();
public:
	void ClearScreen();
	void ScreenTargetSet(bool Clear);
	void Draw();
};