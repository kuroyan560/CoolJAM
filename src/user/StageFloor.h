#pragma once
#include"Singleton.h"
#include<memory>
class ModelObject;
class RenderTarget;

class StageFloor : public Singleton<StageFloor>
{
	friend class Singleton<StageFloor>;

	std::shared_ptr<ModelObject>m_modelObj;
	std::shared_ptr<RenderTarget>m_floorScreen;

	StageFloor();
public:
	void ClearScreen();
	void ScreenTargetSet(bool Clear);
	void Draw();
};