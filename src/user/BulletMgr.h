#pragma once
#include <memory>
#include <array>
#include "Vec.h"

class PlayerBullet;
class EnemyBullet;
class Camera;

class BulletMgr {

private:

	/*===== メンバ変数 =====*/

	std::array<std::shared_ptr<PlayerBullet>, 64> m_playerBullet;
	std::array<std::shared_ptr<EnemyBullet>, 64> m_enemyBullet;


public:

	/*===== メンバ関数 =====*/

	BulletMgr();
	void Init();
	void Update(const float& MapSize);
	void Draw(Camera& NowCam);

	void GeneratePlayerBullet(const Vec3<float>& GeneratePos, const Vec3<float>& ForwardVec);
	void GenerateEnemyBullet(const Vec3<float>& GeneratePos, const Vec3<float>& ForwardVec);

	int CheckHitPlayerBullet(const Vec3<float>& EnemyPos, const float& EnemySize);
	int CheckHitPlayerBulletAngle(const Vec3<float>& EnemyPos, const float& EnemySize, const Vec3<float>& EnemyForwardVec, const float ShieldAngle);
	int CheckHitEnemyBullet(const Vec3<float>& PlayerPos, const float& PlayerSize);

};