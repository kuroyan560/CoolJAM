#include "UnionBaseEnemy.h"
#include "BulletMgr.h"
#include "UnionEnemy.h"

UnionBaseEnemy::UnionBaseEnemy(std::shared_ptr<Model> DefModel, std::shared_ptr<Model> DamageModel)
{

	/*===== �R���X�g���N�^ =====*/

	m_model = DefModel;
	m_modelHit = DamageModel;
	m_isActive = false;

	// UNION�̓G�̎���̓G�𐶐��B
	for (auto& index : m_unionEnemy) {

		index = std::make_shared<UnionEnemy>(m_model, m_modelHit);

	}

}

void UnionBaseEnemy::Init()
{

	/*===== ���������� =====*/

	m_isActive = false;

	for (auto& index : m_unionEnemy) {

		index->Init();

	}

}

void UnionBaseEnemy::Generate(ENEMY_INFO::ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos, const Vec3<float> ForwardVec)
{

	/*===== �������� =====*/

	m_id = ID;
	m_pos = Pos;
	m_knockBackVec = Vec3<float>();
	m_knockBackSpeed = 0.00001f;
	m_shotTimer = 0;
	m_forwardVec = ForwardVec;
	m_speed = 0;
	m_isActive = true;
	m_hitEffectTimer = 0;
	m_scale = UNION_SCALE;
	m_hp = UNION_HP;
	m_transform.SetScale(m_scale);

	// ����̓G�𐶐��B
	const int UNION_COUNT = static_cast<int>(m_unionEnemy.size());
	const float UNION_ROTATE = DirectX::XM_2PI / static_cast<float>(UNION_COUNT);
	float nowRotate = UNION_ROTATE;
	float moveScale = m_scale + m_unionEnemy[0]->GetScale();
	for (int index = 0; index < UNION_COUNT; ++index) {

		m_unionEnemy[index]->Generate(m_pos + Vec3<float>(cosf(nowRotate) * moveScale, 0, sinf(nowRotate) * moveScale));
		nowRotate += UNION_ROTATE;

	}

}

void UnionBaseEnemy::Update(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize)
{

	/*===== �X�V���� =====*/

	for (auto& index : m_unionEnemy) {

		if (!index->GetIsActive()) continue;

		index->Update();

	}

	// �����蔻��
	CheckHitBullet(BulletMgr, MapSize, PlayerPos);

	// �ˏo����
	Shot(BulletMgr, PlayerPos);

	// �}�b�v�O�ɏo����B
	if (MapSize <= m_pos.Length()) {

		m_pos = m_pos.GetNormal() * MapSize;

		--m_hp;
		if (m_hp <= 0) {

			Init();

		}

	}
}

#include"DrawFunc_Append.h"
void UnionBaseEnemy::Draw()
{

	/*===== �`�揈�� =====*/

	m_transform.SetPos(m_pos);
	if (0 < m_hitEffectTimer) {

		//DrawFunc3D::DrawNonShadingModel(m_modelHit, m_transform, Cam);
		DrawFunc_Append::DrawModel(m_modelHit, m_transform);

	}
	else {

		//DrawFunc3D::DrawNonShadingModel(m_model, m_transform, Cam);
		DrawFunc_Append::DrawModel(m_model, m_transform);

	}

	for (auto& index : m_unionEnemy) {

		if (!index->GetIsActive()) continue;

		index->Draw();

	}

}

void UnionBaseEnemy::CheckHitBullet(std::weak_ptr<BulletMgr> BulletMgr, const float& MapSize, const Vec3<float>& PlayerPos)
{

	/*===== �����蔻�� =====*/

	// �}�b�v�O����B
	if (MapSize + MapSize / 2.0f <= m_pos.Length()) {

		m_pos = m_pos.GetNormal() * MapSize;

		Init();

	}

	// �W���̂Ƃ̓����蔻����s���B
	int hitCount = 0;
	int activeCount = 0;
	for (auto& index : m_unionEnemy) {

		if (!index->GetIsActive()) continue;

		// �����蔻����s���B
		Vec3<float> hitBulletPos;
		int hitCount = BulletMgr.lock()->CheckHitPlayerBullet(index->GetPos(), index->GetScale(), hitBulletPos);
		index->Damage(hitCount);

		++activeCount;

	}

	// �{�̂Ƃ̓����蔻��
	Vec3<float> hitBulletPos;
	hitCount = BulletMgr.lock()->CheckHitPlayerBullet(m_pos, m_scale, hitBulletPos);
	if (activeCount <= 0) {

		m_hp -= hitCount;
		hitCount = 0;
		if (m_hp <= 0) {

			Init();

		}

	}

	// �e�ɓ����������t���O
	if (0 < hitCount) {

		m_hitEffectTimer = HIT_EFFECT_TIMER;

	}
	else {

		--m_hitEffectTimer;
		if (m_hitEffectTimer <= 0) {

			m_hitEffectTimer = 0;

		}

	}

}

void UnionBaseEnemy::Shot(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos)
{

	/*===== �e�ˏo���� =====*/

	++m_shotTimer;
	if (SHOT_TIMER < m_shotTimer) {

		// �W���̂̎��肩��e���o���B
		for (auto& index : m_unionEnemy) {

			if (!index->GetIsActive()) continue;

			BulletMgr.lock()->GenerateEnemyBullet(m_pos, Vec3(index->GetPos() - m_pos).GetNormal());

		}

		m_shotTimer = 0;

	}

}