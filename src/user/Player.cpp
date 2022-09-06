#include "Player.h"
#include "../engine/UsersInput.h"
#include "Importer.h"
#include "Model.h"
#include "BulletMgr.h"
#include "EnemyMgr.h"

Player::Player()
{

	/*===== �R���X�g���N�^ =====*/

	m_pos = Vec3<float>();
	m_inputVec = Vec3<float>();
	m_forwardVec = DEF_FORWARDVEC;
	m_speed = 0;
	m_brakeTimer = 0;
	m_shotTimer = 0;
	m_isEdge = false;
	m_isBrake = false;

	m_model = Importer::Instance()->LoadModel("resource/user/", "player.glb");

}

void Player::Init()
{

	/*===== ���������� =====*/

	m_pos = Vec3<float>();
	m_inputVec = Vec3<float>();
	m_forwardVec = DEF_FORWARDVEC;
	m_speed = 0;
	m_brakeTimer = 0;
	m_shotTimer = 0;
	m_isEdge = false;
	m_isBrake = false;

}

void Player::Update(std::weak_ptr<BulletMgr> BulletMgr, std::weak_ptr<EnemyMgr> EnemyMgr, const float& MapSize, const float& EdgeScope)
{

	/*===== �X�V���� =====*/

	// ���͏���
	Input();

	// �ړ�����
	Move();

	// �e���ˏ���
	Shot(BulletMgr, EnemyMgr);

	// �����蔻�菈��
	CheckHit(BulletMgr, MapSize, EdgeScope);

}

void Player::Input()
{

	/*====== ���͏��� =====*/

	// �X�e�B�b�N�̓��͂��擾�B
	Vec2<float> stickInput = UsersInput::Instance()->GetLeftStickVecRawFuna(0);

	// �f�b�h���C����݂���B
	const float INPUT_DEADLINE = 0.25f;
	// ���͂���Ă�����B
	if (INPUT_DEADLINE < stickInput.Length()) {

		// ���͂�ۑ��B
		m_inputVec = Vec3<float>(stickInput.y, 0.0f, stickInput.x);

	}

	// �u���[�L���͂�ۑ��B
	m_isBrake = UsersInput::Instance()->ControllerInput(0, A);
	if (m_isBrake) {

		++m_brakeTimer;

	}
	else {

		// �u���[�L�^�C�}�[��1�ȏゾ������u���[�L�𗣂����u�ԂƂ������ƁB
		if (0 < m_brakeTimer) {

			if (m_isDebugParam) {

				// �o�ߎ��Ԃ��犄�������߂�B
				float brakeRate = Saturate(static_cast<float>(m_brakeTimer) / static_cast<float>(MAX_BRAKE_TIMER)) + 0.5f; // 0.5f ~ 1.5f �͈̔�

				// �ړ����x�����߂�B
				m_speed *= brakeRate;

			}
			else {

				// �o�ߎ��Ԃ��犄�������߂�B
				float brakeRate = Saturate(static_cast<float>(m_brakeTimer) / static_cast<float>(MAX_BRAKE_TIMER)); // 0.5f ~ 1.5f �͈̔�

				// �ړ����x�����߂�B
				m_speed = brakeRate * (MIN_SPEED + MAX_SPEED);

			}

			// �ő�l�A�ŏ��l�𒴂��Ȃ��悤�ɂ���B
			if (m_speed < MIN_SPEED) m_speed = MIN_SPEED;
			if (MAX_SPEED < m_speed) m_speed = MAX_SPEED;

			m_forwardVec = m_inputVec;

		}

		m_brakeTimer = 0;

	}

	// �f�o�b�O�@�\
	if (UsersInput::Instance()->ControllerInput(0, B)) {

		Init();

	}

}

void Player::Move()
{

	/*===== �ړ����� =====*/

	// �u���[�L��Ԃ̗L���ɉ����Ĉړ����x��ς���B
	float speed = m_speed;
	if (m_isBrake) {

		speed = BRAKE_SPEED;

	}

	// �ړ�������B
	m_pos += m_forwardVec * speed;

}

#include"DrawFunc3D.h"
void Player::Draw(Camera& Cam) {

	/*===== �`�揈�� =====*/

	m_transform.SetPos(m_pos);

	// ���͂̊p�x�����߂�B
	Vec2<float> inputVec = m_isBrake ? Vec2<float>(m_inputVec.x, m_inputVec.z) : Vec2<float>(m_forwardVec.x, m_forwardVec.z);
	float inputAngle = atan2f(inputVec.x, inputVec.y);
	m_transform.SetRotate(DirectX::XMMatrixRotationY(inputAngle));

	DrawFunc3D::DrawNonShadingModel(m_model, m_transform, Cam);


}

void Player::Shot(std::weak_ptr<BulletMgr> BulletMgr, std::weak_ptr<EnemyMgr> EnemyMgr) {

	/*===== �e�������� =====*/

	++m_shotTimer;
	if (SHOT_TIMER <= m_shotTimer) {

		m_shotTimer = 0;

		// ��ԋ߂��ɂ���G����������B
		Vec3<float> nearestEnemy = EnemyMgr.lock()->SearchNearestEnemy(m_pos);

		// �G�̕����Ɍ������Ēe�����B
		BulletMgr.lock()->GeneratePlayerBullet(m_pos, (nearestEnemy - m_pos).GetNormal());

	}

}

void Player::DrawDebugInfo(Camera& Cam) {

	/*===== �f�o�b�O����`�� =====*/

	if (m_isBrake) {

		Vec2<float> inputVec = m_isBrake ? Vec2<float>(m_inputVec.x, m_inputVec.z) : Vec2<float>(m_forwardVec.x, m_forwardVec.z);
		float brakeRate = 0;
		if (m_isDebugParam) {

			// �o�ߎ��Ԃ��犄�������߂�B
			brakeRate = Saturate(static_cast<float>(m_brakeTimer) / static_cast<float>(MAX_BRAKE_TIMER)) + 0.5f; // 0.5f ~ 1.5f �͈̔�

			brakeRate = m_speed * brakeRate;
			brakeRate = (m_speed - MIN_SPEED) / (MAX_SPEED - MIN_SPEED);

		}
		else {

			// �o�ߎ��Ԃ��犄�������߂�B
			brakeRate = Saturate(static_cast<float>(m_brakeTimer) / static_cast<float>(MAX_BRAKE_TIMER)); // 0.5f ~ 1.5f �͈̔�


		}

		DrawFunc3D::DrawLine(Cam, m_pos, m_pos + Vec3<float>(inputVec.x, 0.0f, inputVec.y).GetNormal() * (brakeRate * 20.0f), Color(1.0f, 0.0f, 0.0f, 1.0f), 1.0f);

	}

}

void Player::CheckHit(std::weak_ptr<BulletMgr> BulletMgr, const float& MapSize, const float& EdgeScope)
{

	/*===== �����蔻�� =====*/

	// �}�b�v�Ƃ̓����蔻��B
	m_pos = KeepInMap(m_pos, MapSize);

	// �G�b�W�̔���B
	m_isEdge = MapSize - m_pos.Length() < EdgeScope;

	// �G�e�Ƃ̓����蔻��B
	int hitCount = BulletMgr.lock()->CheckHitEnemyBullet(m_pos, SCALE);

}

void Player::Finalize()
{


}


float Player::Saturate(const float& Value)
{

	/*===== 01�ɔ[�߂�B =====*/

	float value = Value;
	if (value < 0.0f) value = 0.0f;
	if (1.0f < value) value = 1.0f;

	return value;

}

Vec3<float> Player::KeepInMap(Vec3<float>& Pos, const float& MapSize)
{

	/*===== �}�b�v���Ɏ��܂�悤�ɂ��� =====*/

	Vec3<float> pos = Pos;
	if (MapSize <= pos.Length()) {

		pos = pos.GetNormal() * MapSize;

	}

	return pos;

}

#include <limits>
bool Player::CheckHitModel(std::weak_ptr<Model> Model, Transform ModelTransform, const Vec3<float>& RayDir, const float& LineLength, Vec3<float>& ShortesetPos)
{

	/*===== ���f���ƃ��C�̐����ł̓����蔻�� =====*/

	// ���f�����璸�_���𔲂����B
	std::vector<Vec3<float>> vertex;
	for (auto& meshIndex : Model.lock()->m_meshes) {

		for (auto& vertexIndex : meshIndex.mesh->vertices) {

			vertex.emplace_back(vertexIndex.pos);

		}

	}

	// ���_��Transform��������B
	DirectX::XMMATRIX matWorld = DirectX::XMMatrixIdentity();
	matWorld *= DirectX::XMMatrixScaling(ModelTransform.GetScale().x, ModelTransform.GetScale().y, ModelTransform.GetScale().z);
	matWorld *= ModelTransform.GetRotate();
	matWorld *= DirectX::XMMatrixTranslation(ModelTransform.GetPos().x, ModelTransform.GetPos().y, ModelTransform.GetPos().z);
	for (auto& index : vertex) {

		index = MulMat(index, matWorld);

	}

	// ���_����ۑ��B
	const int VERTEX_COUNT = static_cast<int>(vertex.size());

	// ���_����0��������A�T�[�g�B
	if (VERTEX_COUNT <= 0) assert(0);

	// �����蔻����s���A�Փ˒n�_��ۑ�����B
	std::vector<Vec2<float>> hitPos;
	// �v���C���[���̃��C�̐ݒ�B
	Vec2<float> pos = Vec2(m_pos.x, m_pos.z);
	Vec2<float> rayDir = Vec2 <float>(RayDir.x, RayDir.z);
	for (auto& index : vertex) {

		// ���f�����̃��C�̐ݒ�B
		Vec2<float> modelPosA = Vec2<float>(index.x, index.z);
		Vec2<float> modelPosB;

		// ���݂̃C���f�b�N�X�̂ЂƂ�̒��_�����߂�B
		int nowIndex = static_cast<int>(&index - &vertex[0]);
		if (VERTEX_COUNT - 1 <= nowIndex) {
			modelPosB = Vec2<float>(vertex[0].x, vertex[0].z);
		}
		else {
			modelPosB = Vec2<float>(vertex[nowIndex + 1].x, vertex[nowIndex + 1].z);
		}

		bool isHit = IsIntersected(pos, pos + rayDir * LineLength, modelPosA, modelPosB);
		if (isHit) {

			hitPos.emplace_back(CalIntersectPoint(pos, pos + rayDir * LineLength, modelPosA, modelPosB));

		}

	}

	// �����������_������Ȃ�������B
	if (static_cast<int>(hitPos.size()) <= 0) {

		return false;

	}
	else {

		// �ŒZ���������߂�B
		float shortestLength = std::numeric_limits<float>().max();
		int shortestIndex = 0;

		for (auto& index : hitPos) {

			float length = (Vec2<float>(m_pos.x, m_pos.z) - index).Length();
			if (shortestLength < length) continue;

			shortestLength = length;
			shortestIndex = static_cast<int>(&index - &hitPos[0]);

		}

		ShortesetPos = Vec3<float>(hitPos[shortestIndex].x, 0, hitPos[shortestIndex].y);

	}

	return true;

}

bool Player::IsIntersected(const Vec2<float>& posA1, const Vec2<float>& posA2, const Vec2<float>& posB1, const Vec2<float>& posB2)
{
	/*--�����̊O�ς��v�Z���Č���������s��--*/
	//���� ����A���猩��B�̌�������
	Vec2<float> buffA = Vec2<float>(posA2.x - posA1.x, posA2.y - posA1.y);
	buffA.Normalize();
	Vec2<float> buffB = Vec2<float>(posB1.x - posA1.x, posB1.y - posA1.y);
	buffB.Normalize();
	Vec2<float> buffC = Vec2<float>(posA2.x - posA1.x, posA2.y - posA1.y);
	buffC.Normalize();
	Vec2<float> buffD = Vec2<float>(posB2.x - posA1.x, posB2.y - posA1.y);
	buffD.Normalize();
	float buffE = buffA.Cross(buffB);
	float buffF = buffC.Cross(buffD);
	float result1 = buffE * buffF;
	bool zero1 = false;
	if (buffE * buffF == 0) zero1 = true;
	//���� ����B���猩��A�̌�������
	buffA = Vec2<float>(posB2.x - posB1.x, posB2.y - posB1.y);
	buffA.Normalize();
	buffB = Vec2<float>(posA1.x - posB1.x, posA1.y - posB1.y);
	buffB.Normalize();
	buffC = Vec2<float>(posB2.x - posB1.x, posB2.y - posB1.y);
	buffC.Normalize();
	buffD = Vec2<float>(posA2.x - posB1.x, posA2.y - posB1.y);
	buffD.Normalize();
	buffE = buffA.Cross(buffB);
	buffF = buffC.Cross(buffD);
	float result2 = buffE * buffF;
	bool zero2 = false;
	if (buffE * buffF == 0) zero2 = true;

	//�������������Ă��鎞�́A�������猩�Č������肵���������̒[�_2�������ɂ��鎞�B
	//�O�ςō��E���������ƁA�������Ă��鎞�͒l�̌��ʂ�+��-�ɂȂ�B
	//�܂藼���̊O�ς��|���Č��ʂ�-�ɂȂ����ꍇ�̂݌������Ă���B
	//����A����݂�B�𔻒�A����B����݂�A�𔻒�Ɠ�ʂ蔻����s���B
	//����2�̔��茋�ʂ��|��������-�A�Ⴕ����0�̎�2�̐����͌������Ă���B
	if ((result1 <= 0 && result2 <= 0) ||
		(result1 <= 0 && zero2) ||
		(zero1 && result2 <= 0)) {
		return true;
	}
	return false;
}

Vec2<float> Player::CalIntersectPoint(Vec2<float> posA1, Vec2<float> posA2, Vec2<float> posB1, Vec2<float> posB2)
{
	//��_�����߂� ���̎��͎������̂܂�
	Vec2<float> buff = Vec2<float>(posB2.x - posB1.x, posB2.y - posB1.y);
	double d1 = abs(buff.Cross(Vec2<float>(posA1.x - posB1.x, posA1.y - posB1.y)));
	double d2 = abs(buff.Cross(Vec2<float>(posA2.x - posB1.x, posA2.y - posB1.y)));
	double t = d1 / (d1 + d2);

	return Vec2<float>(posA1.x + (posA2.x - posA1.x) * t, posA1.y + (posA2.y - posA1.y) * t);
}
