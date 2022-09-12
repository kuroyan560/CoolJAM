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
	void Init()override;
	void Generate(ENEMY_INFO::ID ID, const Vec3<float> &PlayerPos, const Vec3<float> &Pos, const Vec3<float> ForwardVec)override;
	void Update(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float> &PlayerPos, const float &MapSize)override;
	void Draw()override;

private:
};

