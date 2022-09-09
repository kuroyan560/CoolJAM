#include "BulletMgr.h"
#include "../engine/UsersInput.h"
#include "Importer.h"
#include "Model.h"
#include "PlayerBullet.h"
#include "EnemyBullet.h"

BulletMgr::BulletMgr() {

	/*====== �R���X�g���N�^ =====*/

	for (auto& index : m_enemyBullet) {

		index = std::make_shared<EnemyBullet>();

	}

	for (auto& index : m_playerBullet) {

		index = std::make_shared<PlayerBullet>();

	}

}

void BulletMgr::Init() {

	/*====== ���������� =====*/

	for (auto& index : m_enemyBullet) {

		index->Init();

	}

	for (auto& index : m_playerBullet) {

		index->Init();

	}

}

void BulletMgr::Update(const float& MapSize) {

	/*====== �X�V���� =====*/

	for (auto& index : m_enemyBullet) {

		if (!index->GetIsActive()) continue;

		index->Update(MapSize);

	}

	for (auto& index : m_playerBullet) {

		if (!index->GetIsActive()) continue;

		index->Update(MapSize);

	}

}

void BulletMgr::Draw(Camera& NowCam) {

	/*====== �`�揈�� =====*/

	for (auto& index : m_enemyBullet) {

		if (!index->GetIsActive()) continue;

		index->Draw(NowCam);

	}

	for (auto& index : m_playerBullet) {

		if (!index->GetIsActive()) continue;

		index->Draw(NowCam);

	}

}

void BulletMgr::GeneratePlayerBullet(const Vec3<float>& GeneratePos, const Vec3<float>& ForwardVec) {

	/*===== �v���C���[�e�𐶐� =====*/

	for (auto& index : m_playerBullet) {

		if (index->GetIsActive()) continue;

		index->Generate(GeneratePos, ForwardVec);

		break;

	}

}

void BulletMgr::GenerateEnemyBullet(const Vec3<float>& GeneratePos, const Vec3<float>& ForwardVec) {

	/*===== �G�e�𐶐� =====*/

	for (auto& index : m_enemyBullet) {

		if (index->GetIsActive()) continue;

		index->Generate(GeneratePos, ForwardVec);

		break;

	}

}

int BulletMgr::CheckHitPlayerBullet(const Vec3<float>& EnemyPos, const float& EnemySize) {

	/*===== �v���C���[�e�Ƃ̓����蔻�� =====*/

	int hitCount = 0;
	for (auto& index : m_playerBullet) {

		if (!index->GetIsActive()) continue;

		// �����蔻����s���B
		bool isHit = Vec3<float>(EnemyPos - index->GetPos()).Length() <= index->GetCheckHitScale() + EnemySize;
		if (!isHit) continue;

		++hitCount;
		index->Init();

	}

	return hitCount;

}

int BulletMgr::CheckHitPlayerBulletAngle(const Vec3<float>& EnemyPos, const float& EnemySize, const Vec3<float>& EnemyForwardVec, const float ShieldAngle)
{

	/*===== �v���C���[�e�Ƃ̓����蔻�� =====*/

	int hitCount = 0;
	for (auto& index : m_playerBullet) {

		if (!index->GetIsActive()) continue;

		// �����蔻����s���B
		bool isHit = Vec3<float>(EnemyPos - index->GetPos()).Length() <= index->GetCheckHitScale() + EnemySize;
		if (!isHit) continue;

		// �p�x�̍������߂�B
		float dot = Vec3<float>(index->GetPos() - EnemyPos).GetNormal().Dot(EnemyForwardVec);
		// ���������ȏゾ������B
		if (dot < ShieldAngle) {

			++hitCount;
			index->Init();

		}
		else {

			index->Init();
			continue;

		}

	}

	return hitCount;

}

int BulletMgr::CheckHitEnemyBullet(const Vec3<float>& PlayerPos, const float& PlayerSize) {

	/*===== �G�e�Ƃ̓����蔻�� =====*/

	int hitCount = 0;
	for (auto& index : m_enemyBullet) {

		if (!index->GetIsActive()) continue;

		// �����蔻����s���B
		bool isHit = Vec3<float>(PlayerPos - index->GetPos()).Length() <= index->GetCheckHitScale() + PlayerSize;
		if (!isHit) continue;

		++hitCount;
		index->Init();

	}

	return hitCount;

}