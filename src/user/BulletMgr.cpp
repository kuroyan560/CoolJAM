#include "BulletMgr.h"
#include "../engine/UsersInput.h"
#include "Importer.h"
#include "Model.h"
#include "PlayerBullet.h"
#include "EnemyBullet.h"

BulletMgr::BulletMgr() {

	/*====== コンストラクタ =====*/

	for (auto& index : m_enemyBullet) {

		index = std::make_shared<EnemyBullet>();

	}

	for (auto& index : m_playerBullet) {

		index = std::make_shared<PlayerBullet>();

	}

}

void BulletMgr::Init() {

	/*====== 初期化処理 =====*/

	for (auto& index : m_enemyBullet) {

		index->Init();

	}

	for (auto& index : m_playerBullet) {

		index->Init();

	}

}

void BulletMgr::Update(const float& MapSize) {

	/*====== 更新処理 =====*/

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

	/*====== 描画処理 =====*/

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

	/*===== プレイヤー弾を生成 =====*/

	for (auto& index : m_playerBullet) {

		if (index->GetIsActive()) continue;

		index->Generate(GeneratePos, ForwardVec);

		break;

	}

}

void BulletMgr::GenerateEnemyBullet(const Vec3<float>& GeneratePos, const Vec3<float>& ForwardVec) {

	/*===== 敵弾を生成 =====*/

	for (auto& index : m_enemyBullet) {

		if (index->GetIsActive()) continue;

		index->Generate(GeneratePos, ForwardVec);

		break;

	}

}

int BulletMgr::CheckHitPlayerBullet(const Vec3<float>& EnemyPos, const float& EnemySize, Vec3<float>& HitBulletPos) {

	/*===== プレイヤー弾との当たり判定 =====*/

	int hitCount = 0;
	for (auto& index : m_playerBullet) {

		if (!index->GetIsActive()) continue;

		// 当たり判定を行う。
		bool isHit = Vec3<float>(EnemyPos - index->GetPos()).Length() <= index->GetCheckHitScale() + EnemySize;
		if (!isHit) continue;

		++hitCount;
		HitBulletPos = index->GetPos();
		index->Init();

	}

	return hitCount;

}

int BulletMgr::CheckHitPlayerBulletAngle(const Vec3<float>& EnemyPos, const float& EnemySize, const Vec3<float>& EnemyForwardVec, const float ShieldAngle)
{

	/*===== プレイヤー弾との当たり判定 =====*/

	int hitCount = 0;
	for (auto& index : m_playerBullet) {

		if (!index->GetIsActive()) continue;

		// 当たり判定を行う。
		bool isHit = Vec3<float>(EnemyPos - index->GetPos()).Length() <= index->GetCheckHitScale() + EnemySize;
		if (!isHit) continue;

		// 角度の差を求める。
		float dot = Vec3<float>(index->GetPos() - EnemyPos).GetNormal().Dot(EnemyForwardVec);
		// 差が引数以上だったら。
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

	/*===== 敵弾との当たり判定 =====*/

	int hitCount = 0;
	for (auto& index : m_enemyBullet) {

		if (!index->GetIsActive()) continue;

		// 当たり判定を行う。
		bool isHit = Vec3<float>(PlayerPos - index->GetPos()).Length() <= index->GetCheckHitScale() + PlayerSize;
		if (!isHit) continue;

		++hitCount;
		index->Init();

	}

	return hitCount;

}