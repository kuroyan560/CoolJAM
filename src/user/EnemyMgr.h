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
	std::shared_ptr<Model> m_elecMushiHit;

	Color m_particleColor;
public:

	/*===== ƒƒ“ƒoŠÖ” =====*/

	EnemyMgr();
	void Init();
	void Update(std::weak_ptr< BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize);
	void Draw(Camera &NowCam, std::weak_ptr<RenderTarget>Main, std::weak_ptr<RenderTarget>EmmisiveMap, std::weak_ptr<DepthStencil>DepthStencil);
	
	//‘ŞU
	void AllDisappear();

	// ˆê”Ô‹ß‚­‚É‚¢‚é“G‚ÌêŠ‚ğ‚©‚¦‚·B
	Vec3<float> SearchNearestEnemy(const Vec3<float>& Pos);

	// w’è‚ÌƒxƒNƒgƒ‹‚Éˆê”Ô‹ß‚¢“G‚ğ’T‚·B
	Vec3<float> SearchNearestEnemyToVector(const Vec3<float>& Pos, const Vec3<float>& Vec, const float& ReceivingRate);

	// “G‚Ì‹ß‚­‚É‚¢‚é”»’èB
	//bool CheckEnemyEdge(const Vec3<float>& Pos, const float& Size);

	// “G‚Æ‚ÌÕ“Ë”»’èB
	bool CheckHitEnemy(const Vec3<float>& Pos, const float& Size);

	// w’è‚Ì”ÍˆÍ‚Ì“G‚ğ“|‚·B
	int AttackEnemy(const Vec3<float>& Pos, const float& Size, std::weak_ptr<BulletMgr> BulletMgr);

	// “G‚ğ¶¬B
	void Generate(const Vec3<float>& PlayerPos, const Vec3<float>& GeneratePos, const Vec3<float> ForwardVec, const int& EnemyID, const int& ShotTimer, const float& MapSize);

	// ‚·‚×‚Ä‚Ì“G‚ª€‚ñ‚Å‚¢‚é‚©B
	bool GetAllEnemyDead();

	// ‚·‚×‚Ä‚Ì“G‚ğE‚·B
	void AllKill(std::weak_ptr<BulletMgr> BulletMgr);

	Color GetParticleColor();
private:

	// “G¶¬ŠÖ” Generate“à‚Åg—p‚·‚éB
	void GenerateEnemy(std::shared_ptr<BaseEnemy>& Enemy, const Vec3<float>& PlayerPos, const Vec3<float>& GeneratePos, const Vec3<float> ForwardVec, const int& EnemyID, const int& ShotTimer, const float& MapSize);

};