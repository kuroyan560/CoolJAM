#pragma once
#include "Vec.h"
#include "EnemyWave.h"
#include <memory>

class Model;
class Camera;
class BulletMgr;

class BaseEnemy {

public:

	/*===== �����o�ϐ� =====*/

	virtual void Init() = 0;
	virtual void Generate(ENEMY_INFO::ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos, const Vec3<float> ForwardVec) = 0;
	virtual void Update(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize) = 0;
	virtual void Draw(Camera& Cam) = 0;
	virtual bool GetIsActive() = 0;
	virtual float GetScale() = 0;
	virtual Vec3<float> GetPos() = 0;

	ENEMY_INFO::ID GetId()
	{
		return baseEnemy_id;
	};

protected:
	ENEMY_INFO::ID baseEnemy_id;

};