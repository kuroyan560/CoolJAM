#pragma once
#include <array>
#include <memory>
#include "Vec.h"

class Enemy;
class Camera;
class BulletMgr;

class EnemyMgr {

private:

	/*===== ƒƒ“ƒo•Ï” =====*/

	std::array<std::shared_ptr<Enemy>, 64> m_enemy;


public:

	/*===== ƒƒ“ƒoŠÖ” =====*/

	EnemyMgr();
	void Init();
	void Update(std::weak_ptr< BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize);
	void Draw(Camera& NowCam);

	// ˆê”Ô‹ß‚­‚É‚¢‚é“G‚ÌêŠ‚ğ‚©‚¦‚·B
	Vec3<float> SearchNearestEnemy(const Vec3<float>& Pos);

	// “G‚Ì‹ß‚­‚É‚¢‚é”»’èB
	bool CheckEnemyEdge(const Vec3<float>& Pos, const float& Size);

	// “G‚Æ‚ÌÕ“Ë”»’èB
	bool CheckHitEnemy(const Vec3<float>& Pos, const float& Size);

	// w’è‚Ì”ÍˆÍ‚Ì“G‚ğ“|‚·B
	void AttackEnemy(const Vec3<float>& Pos, const float& Size);

	// “G‚ğ¶¬B
	void Generate(const Vec3<float>& PlayerPos, const Vec3<float>& GeneratePos, const Vec3<float> ForwardVec, const int& EnemyID, const float& MapSize);


private:

};