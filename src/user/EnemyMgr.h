#pragma once
#include <array>
#include <memory>
#include "Vec.h"
#include"EnemyDeadEmitter.h"

class BaseEnemy;
class Camera;
class BulletMgr;
class Model;

class EnemyMgr {

private:

	/*===== ƒƒ“ƒo•Ï” =====*/

	std::array<std::shared_ptr<BaseEnemy>, 64> m_enemy;
	std::array<EnemyDeadEmitter, 64> m_deadEffectEmitterArray;
	std::array<bool, 64> m_initDeadEffectArray;

	std::shared_ptr<Model> m_model;
	std::shared_ptr<Model> m_modelHit;
	std::shared_ptr<Model> m_elecMushi;


public:

	/*===== ƒƒ“ƒoŠÖ” =====*/

	EnemyMgr();
	void Init();
	void Update(std::weak_ptr< BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize);
	void Draw(Camera &NowCam, std::weak_ptr<RenderTarget>Main, std::weak_ptr<RenderTarget>EmmisiveMap, std::weak_ptr<DepthStencil>DepthStencil);

	// ˆê”Ô‹ß‚­‚É‚¢‚é“G‚ÌêŠ‚ğ‚©‚¦‚·B
	Vec3<float> SearchNearestEnemy(const Vec3<float>& Pos);

	// “G‚Ì‹ß‚­‚É‚¢‚é”»’èB
	//bool CheckEnemyEdge(const Vec3<float>& Pos, const float& Size);

	// “G‚Æ‚ÌÕ“Ë”»’èB
	bool CheckHitEnemy(const Vec3<float>& Pos, const float& Size);

	// w’è‚Ì”ÍˆÍ‚Ì“G‚ğ“|‚·B
	void AttackEnemy(const Vec3<float>& Pos, const float& Size);

	// “G‚ğ¶¬B
	void Generate(const Vec3<float>& PlayerPos, const Vec3<float>& GeneratePos, const Vec3<float> ForwardVec, const int& EnemyID, const float& MapSize);


private:

	// “G¶¬ŠÖ” Generate“à‚Åg—p‚·‚éB
	void GenerateEnemy(std::shared_ptr<BaseEnemy>& Enemy, const Vec3<float>& PlayerPos, const Vec3<float>& GeneratePos, const Vec3<float> ForwardVec, const int& EnemyID, const float& MapSize);

};