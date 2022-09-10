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

	/*===== �����o�ϐ� =====*/

	std::array<std::shared_ptr<BaseEnemy>, 64> m_enemy;
	std::array<EnemyDeadEmitter, 64> m_deadEffectEmitterArray;
	std::array<bool, 64> m_initDeadEffectArray;

	std::shared_ptr<Model> m_model;
	std::shared_ptr<Model> m_modelHit;
	std::shared_ptr<Model> m_elecMushi;


public:

	/*===== �����o�֐� =====*/

	EnemyMgr();
	void Init();
	void Update(std::weak_ptr< BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize);
	void Draw(Camera &NowCam, std::weak_ptr<RenderTarget>Main, std::weak_ptr<RenderTarget>EmmisiveMap, std::weak_ptr<DepthStencil>DepthStencil);

	// ��ԋ߂��ɂ���G�̏ꏊ���������B
	Vec3<float> SearchNearestEnemy(const Vec3<float>& Pos);

	// �G�̋߂��ɂ��锻��B
	//bool CheckEnemyEdge(const Vec3<float>& Pos, const float& Size);

	// �G�Ƃ̏Փ˔���B
	bool CheckHitEnemy(const Vec3<float>& Pos, const float& Size);

	// �w��͈̔͂̓G��|���B
	void AttackEnemy(const Vec3<float>& Pos, const float& Size);

	// �G�𐶐��B
	void Generate(const Vec3<float>& PlayerPos, const Vec3<float>& GeneratePos, const Vec3<float> ForwardVec, const int& EnemyID, const float& MapSize);


private:

	// �G�����֐� Generate���Ŏg�p����B
	void GenerateEnemy(std::shared_ptr<BaseEnemy>& Enemy, const Vec3<float>& PlayerPos, const Vec3<float>& GeneratePos, const Vec3<float> ForwardVec, const int& EnemyID, const float& MapSize);

};