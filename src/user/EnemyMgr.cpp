#include "EnemyMgr.h"
#include "Enemy.h"
#include "KuroFunc.h"
#include <limits>

EnemyMgr::EnemyMgr()
{

	/*===== �R���X�g���N�^ =====*/

	for (auto& index : m_enemy) {

		index = std::make_shared<Enemy>();

	}

}

void EnemyMgr::Init()
{

	/*===== ���������� =====*/

	for (auto& index : m_enemy) {

		index->Init();


	}

}

void EnemyMgr::Update(std::weak_ptr< BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize)
{

	/*===== �X�V���� =====*/

	for (auto& index : m_enemy) {

		if (!index->GetIsActive()) continue;

		index->Update(BulletMgr, PlayerPos, MapSize);

	}

}

void EnemyMgr::Draw(Camera& NowCam)
{

	/*===== �`�揈�� =====*/

	for (auto& index : m_enemy) {

		if (!index->GetIsActive()) continue;

		index->Draw(NowCam);

	}

}

void EnemyMgr::Generate(const Vec3<float>& PlayerPos, const Vec3<float>& GeneratePos, const Vec3<float> ForwardVec, const int& EnemyID, const float& MapSize)
{

	/*===== �������� =====*/

	for (auto& index : m_enemy) {

		if (index->GetIsActive()) continue;

		// �G��ID
		ENEMY_INFO::ID enemyID = static_cast<ENEMY_INFO::ID>(EnemyID);

		// ��������B
		index->Generate(enemyID, PlayerPos, GeneratePos, ForwardVec);

		break;

	}

}

Vec3<float> EnemyMgr::SearchNearestEnemy(const Vec3<float>& Pos) {

	/*===== ��ԋ߂��ɂ���G�̍��W�����߂� =====*/

	float nearestLength = std::numeric_limits<float>().max();
	Vec3<float> nearestPos = Vec3<float>();
	for (auto& index : m_enemy) {

		if (!index->GetIsActive()) continue;

		// ���������߂�B
		float length = Vec3<float>(index->GetPos() - Pos).Length();
		if (nearestLength < length) continue;

		nearestLength = length;
		nearestPos = index->GetPos();

	}

	return nearestPos;

}

bool EnemyMgr::CheckEnemyEdge(const Vec3<float>& Pos, const float& Size) {

	/*===== �G�b�W�̔��� =====*/

	for (auto& index : m_enemy) {

		if (!index->GetIsActive()) continue;

		if (index->CheckIsEdge(Pos, Size)) {

			return true;

		}

	}

	return false;

}

void EnemyMgr::AttackEnemy(const Vec3<float>& Pos, const float& Size) {

	/*===== �w��͈̔͂̓G��|�� =====*/

	for (auto& index : m_enemy) {

		if (!index->GetIsActive()) continue;

		// �G�̍��W
		Vec3<float> enemyPos = index->GetPos();
		float enemySize = index->GetScale();

		// �����蔻��
		if (!(Vec3<float>(enemyPos - Pos).Length() <= Size + enemySize)) continue;

		index->Init();

	}

}