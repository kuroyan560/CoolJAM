#pragma once
#include "Vec.h"
#include "EnemyWave.h"
#include "BaseEnemy.h"
#include "Transform.h"
#include <memory>

class Model;
class Camera;
class BulletMgr;

class CoinEnemy :public BaseEnemy
{
public:
	CoinEnemy(std::shared_ptr<Model> DefModel, std::shared_ptr<Model> DamageModel);
	void Init();
	void Generate(ENEMY_INFO::ID ID, const Vec3<float> &PlayerPos, const Vec3<float> &Pos, const Vec3<float> ForwardVec);
	void Update(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float> &PlayerPos, const float &MapSize);
	void Draw(Camera &Cam);
	bool GetIsActive();
	float GetScale();
	Vec3<float> GetPos();

private:
	Vec3<float>m_pos;


	ENEMY_INFO::ID m_id;
	std::shared_ptr<Model> m_model;
	std::shared_ptr<Model> m_modelHit;
	Transform m_transform;
	bool m_isActive;
	float m_scale;
};

