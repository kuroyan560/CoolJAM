#include "Enemy.h"
#include "Importer.h"
#include "Model.h"
#include "BulletMgr.h"
#include "UnionEnemy.h"

std::shared_ptr<Model> Enemy::s_model;
std::shared_ptr<Model> Enemy::s_modelHit;
bool Enemy::m_debugIsShotEnemy = true;

Enemy::Enemy() {

	/*===== �R���X�g���N�^ =====*/

	m_pos = Vec3<float>();
	m_forwardVec = Vec3<float>(0, 0, 1);
	m_scale = 1.0f;
	m_isActive = false;
	m_hitEffectTimer = 0;
	m_shotTimer = 0;
	m_hp = 0;
	m_knockBackSpeed = 0.00001f;
	m_knockBackVec = Vec3<float>();

	if (!s_model)
	{
		s_model = Importer::Instance()->LoadModel("resource/user/", "enemy.glb");
		s_modelHit = Importer::Instance()->LoadModel("resource/user/", "enemy_hit.glb");
	}
	m_transform.SetScale(1.0f);

	// UNION�̓G�̎���̓G�𐶐��B
	for (auto& index : m_unionEnemy) {

		index = std::make_shared<UnionEnemy>(s_model, s_modelHit);

	}

}

void Enemy::Init()
{

	/*===== ���������� =====*/

	m_pos = Vec3<float>();
	m_forwardVec = Vec3<float>(0, 0, 1);
	m_scale = 1.0f;
	m_isActive = false;
	m_hitEffectTimer = 0;
	m_shotTimer = 0;
	m_hp = 0;
	m_knockBackSpeed = 0.00001f;
	m_knockBackVec = Vec3<float>();

	m_transform.SetScale(1.0f);

	for (auto& index : m_unionEnemy) {

		index->Init();

	}

}

void Enemy::Generate(ENEMY_INFO::ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos, const Vec3<float> ForwardVec)
{

	/*===== ���������� =====*/

	m_id = ID;
	m_pos = Pos;
	m_shotTimer = 0;
	m_forwardVec = ForwardVec;
	m_speed = 0;
	m_isActive = true;
	m_hitEffectTimer = 0;
	m_knockBackSpeed = 0.00001f;
	m_knockBackVec = Vec3<float>();

	switch (m_id)
	{
	case ENEMY_INFO::ID::STOPPING:
		/*== �~�܂��Ă���G ==*/
		m_scale = STOPPING_SCALE;
		m_hp = STOPPING_HP;
		break;
	case ENEMY_INFO::ID::STRAIGHT:
		/*== �܂������i�ޓG ==*/
		m_scale = STRAIGHT_SCALE;
		m_hp = STRAIGHT_HP;
		break;
	case ENEMY_INFO::ID::PLAYER_STRAIGHT:
		/*== �v���C���[�Ɍ������Ă܂������i�ޓG ==*/
		m_scale = PLAYER_STRAIGHT_SCALE;
		m_hp = PLAYER_STRAIGHT_HP;
		break;
	case ENEMY_INFO::ID::TRACKING:
		/*== �v���C���[��ǔ�����G ==*/
		m_scale = TRACKING_SCALE;
		m_hp = TRACKING_HP;
		m_forwardVec = Vec3<float>(PlayerPos - m_pos).GetNormal();
		break;
	case ENEMY_INFO::ID::SHIELD:
		/*== �����������G ==*/
		m_scale = SHIELD_SCALE;
		m_hp = SHIELD_HP;
		break;
	case ENEMY_INFO::ID::UNION:
		/*== �W���̂̓G ==*/
	{
		m_scale = UNION_SCALE;
		m_hp = UNION_HP;
		const int UNION_COUNT = static_cast<int>(m_unionEnemy.size());
		const float UNION_ROTATE = DirectX::XM_2PI / static_cast<float>(UNION_COUNT);
		float nowRotate = UNION_ROTATE;
		float moveScale = m_scale + m_unionEnemy[0]->GetScale();
		for (int index = 0; index < UNION_COUNT; ++index) {

			m_unionEnemy[index]->Generate(m_pos + Vec3<float>(cosf(nowRotate) * moveScale, 0, sinf(nowRotate) * moveScale));
			nowRotate += UNION_ROTATE;

		}
	}

	break;
	case ENEMY_INFO::ID::TORUS_MOVE:
		/*== �~��ɓ����G ==*/
		m_scale = STRAIGHT_SCALE;
		m_hp = STRAIGHT_HP;
		break;
	case ENEMY_INFO::ID::PRESS:
		/*== �����ē|���G ==*/
		m_scale = PRESS_SCALE;
		m_hp = PRESS_HP;
		break;
	case ENEMY_INFO::ID::DASH:
		/*== �~��ɓ����G ==*/
		m_scale = STRAIGHT_SCALE;
		m_hp = STRAIGHT_HP;
		m_forwardVec = Vec3<float>(PlayerPos - m_pos).GetNormal();
		break;
	default:
		// �p�����[�^���ݒ肳��Ă��Ȃ��ł��I
		assert(0);
		break;
	}

	m_transform.SetScale(m_scale);

}

void Enemy::Update(std::weak_ptr< BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize)
{

	/*===== �X�V���� =====*/

	switch (m_id)
	{
	case ENEMY_INFO::ID::STOPPING:
		break;
	case ENEMY_INFO::ID::STRAIGHT:
	case ENEMY_INFO::ID::PLAYER_STRAIGHT:

		/*-- �ǔ��G --*/

		// �ړ����x���ԁB
		m_speed += (SPEED - m_speed) / 10.0f;

		// �ړ�������B
		m_pos += m_forwardVec * m_speed;


		break;
	case ENEMY_INFO::ID::TRACKING:

		/*-- �ǔ��G --*/

	{

		// ���E���������B
		float cross = m_forwardVec.Cross(PlayerPos - m_pos).y;
		if (cross != 0) {

			cross = cross < 0 ? -1.0f : 1.0f;
			float handleRot = TRACKING_ROT * cross;

			// �ړ������x�N�g�����p�x�ɒ����Ēl�����Z����B
			float forwardVecAngle = atan2f(m_forwardVec.x, m_forwardVec.z);
			forwardVecAngle += handleRot;

			// ���Z�����p�x���x�N�g���ɒ����B
			m_forwardVec = Vec3<float>(sinf(forwardVecAngle), 0.0f, cosf(forwardVecAngle));

		}

		// �ړ����x���ԁB
		m_speed += (TRACKING_SPEED - m_speed) / 10.0f;

		// �ړ�������B
		m_pos += m_forwardVec * m_speed;


	}

	break;
	case ENEMY_INFO::ID::SHIELD:

		/*-- �������G --*/
	{

		// ���E���������B
		float cross = m_forwardVec.Cross(PlayerPos - m_pos).y;
		if (cross != 0) {

			cross = cross < 0 ? -1.0f : 1.0f;
			float handleRot = SHIELD_ROT * cross;

			// �ړ������x�N�g�����p�x�ɒ����Ēl�����Z����B
			float forwardVecAngle = atan2f(m_forwardVec.x, m_forwardVec.z);
			forwardVecAngle += handleRot;

			// ���Z�����p�x���x�N�g���ɒ����B
			m_forwardVec = Vec3<float>(sinf(forwardVecAngle), 0.0f, cosf(forwardVecAngle));

		}

		// �`�����]������B
		float angle = atan2f(m_forwardVec.x, m_forwardVec.z);
		m_transform.SetRotate(DirectX::XMMatrixRotationY(angle));

	}

	break;

	case ENEMY_INFO::ID::UNION:

		/*-- �W���̂̓G --*/

		for (auto& index : m_unionEnemy) {

			if (!index->GetIsActive()) continue;

			index->Update();

		}

		break;

	case ENEMY_INFO::ID::TORUS_MOVE:

		/*== �~��ɓ����G ==*/

	{

		// ���S����̋���
		float centerDistance = m_pos.Length();

		// �ړ�����O�̍��W
		Vec3<float> prevPos = m_pos;

		// �ړ�������B
		m_speed = SPEED;
		m_pos += m_forwardVec * m_speed;

		// �����߂��B
		if (centerDistance <= m_pos.Length()) {

			m_pos = m_pos.GetNormal() * centerDistance;

		}

		// ���ʃx�N�g������]�B
		m_forwardVec = Vec3<float>(m_pos - prevPos).GetNormal();

		break;

	}
	case ENEMY_INFO::ID::DASH:

		/*== �_�b�V������G ==*/
	{

		// ���E���������B
		float cross = m_forwardVec.Cross(PlayerPos - m_pos).y;
		if (cross != 0) {

			cross = cross < 0 ? -1.0f : 1.0f;
			float handleRot = TRACKING_ROT * cross;

			// �ړ������x�N�g�����p�x�ɒ����Ēl�����Z����B
			float forwardVecAngle = atan2f(m_forwardVec.x, m_forwardVec.z);
			forwardVecAngle += handleRot;

			// ���Z�����p�x���x�N�g���ɒ����B
			m_forwardVec = Vec3<float>(sinf(forwardVecAngle), 0.0f, cosf(forwardVecAngle));

		}


	}
	break;


	default:
		break;
	}

	// �m�b�N�o�b�N�̈ړ��B
	m_pos += m_knockBackVec * m_knockBackSpeed;

	// �m�b�N�o�b�N�̈ړ������炷�B
	m_knockBackSpeed -= m_knockBackSpeed / 20.0f;

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

#include"DrawFunc3D.h"
void Enemy::Draw(Camera& Cam)
{

	/*===== �`�揈�� =====*/

	m_transform.SetPos(m_pos);
	if (0 < m_hitEffectTimer) {

		DrawFunc3D::DrawNonShadingModel(s_modelHit, m_transform, Cam);

	}
	else {

		DrawFunc3D::DrawNonShadingModel(s_model, m_transform, Cam);

	}

	// ID���W���̂̓G��������W���̂�`�悷��B
	if (m_id == ENEMY_INFO::ID::UNION) {

		for (auto& index : m_unionEnemy) {

			if (!index->GetIsActive()) continue;

			index->Draw(Cam);

		}

	}

}

void Enemy::CheckHitBullet(std::weak_ptr< BulletMgr> BulletMgr, const float& MapSize, const Vec3<float>& PlayerPos)
{

	/*===== �����蔻�� =====*/

	// �}�b�v�O����B
	if (MapSize + MapSize / 2.0f <= m_pos.Length()) {

		m_pos = m_pos.GetNormal() * MapSize;

		Init();

	}

	int hitCount = 0;
	// �������̓G��������B
	if (m_id == ENEMY_INFO::ID::SHIELD) {

		// �v���C���[�e�Ƃ̓����蔻��B
		hitCount = BulletMgr.lock()->CheckHitPlayerBulletAngle(m_pos, m_scale, m_forwardVec, 0.0f);
		m_hp -= hitCount;
		if (m_hp <= 0) {

			Init();

		}

	}
	// �W���̂̓G��������B
	else if (m_id == ENEMY_INFO::ID::UNION) {

		// �W���̂Ƃ̓����蔻����s���B
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

	}
	// �����ē|���G��������B
	if (m_id == ENEMY_INFO::ID::PRESS) {

		// �v���C���[�e�Ƃ̓����蔻��B
		Vec3<float> hitBulletPos;
		hitCount = BulletMgr.lock()->CheckHitPlayerBullet(m_pos, m_scale, hitBulletPos);

		// ID��TRACK��������m�b�N�o�b�N������B
		if (0 < hitCount && m_id == ENEMY_INFO::ID::PRESS) {

			m_knockBackVec = Vec3<float>(m_pos - PlayerPos).GetNormal();
			m_knockBackSpeed = PRESS_KNOCK_BACK_SPEED;

		}

	}
	else {

		// �v���C���[�e�Ƃ̓����蔻��B
		Vec3<float> hitBulletPos;
		hitCount = BulletMgr.lock()->CheckHitPlayerBullet(m_pos, m_scale, hitBulletPos);

		// ID��TRACK��������m�b�N�o�b�N������B
		if (0 < hitCount && (m_id == ENEMY_INFO::ID::TRACKING || m_id == ENEMY_INFO::ID::DASH)) {

			m_knockBackVec = Vec3<float>(m_pos - PlayerPos).GetNormal();
			m_knockBackSpeed = KNOCK_BACK_SPEED;

		}

		m_hp -= hitCount;
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

void Enemy::Shot(std::weak_ptr< BulletMgr> BulletMgr, const Vec3<float>& PlayerPos) {

	/*===== �e�ˏo���� =====*/

	if (!m_debugIsShotEnemy) return;
	if (!(m_id == ENEMY_INFO::ID::UNION)) return;

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

Vec3<float> Enemy::KeepInMap(Vec3<float>& Pos, const float& MapSize)
{

	/*===== �}�b�v���Ɏ��܂�悤�ɂ��� =====*/

	Vec3<float> pos = Pos;
	if (MapSize <= pos.Length()) {

		pos = pos.GetNormal() * MapSize;

	}

	return pos;

}

bool Enemy::CheckIsEdge(const Vec3<float>& Pos, const float& Size) {

	/*===== �߂��ɂ��邩�ǂ��� =====*/

	// ��_�Ԃ̋���
	float length = Vec3<float>(Pos - m_pos).Length();

	// �G�b�W�p�̉~�Ƃ̓����蔻��B
	if (length < Size + EDGE) {

		return true;

	}

	return false;

}

bool Enemy::CheckHit(const Vec3<float>& Pos, const float& Size)
{

	/*===== �����蔻�� =====*/

	// ��_�Ԃ̋���
	float length = Vec3<float>(Pos - m_pos).Length();

	if (length < Size + m_scale) {

		return true;

	}

	return false;

}

void Enemy::Damage() {

	/*===== �_���[�W��^���� =====*/

	--m_hp;
	if (m_hp <= 0) {

		Init();

	}

}