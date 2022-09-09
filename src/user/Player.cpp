#include "Player.h"
#include "../engine/UsersInput.h"
#include "Importer.h"
#include "Model.h"
#include "BulletMgr.h"
#include "EnemyMgr.h"
#include "KuroFunc.h"
#include "GameManager.h"
#include "DriftParticle.h"

Player::Player()
{

	/*===== �R���X�g���N�^ =====*/

	m_pos = Vec3<float>();
	m_inputVec = Vec3<float>();
	m_forwardVec = DEF_FORWARDVEC;
	m_prevForwardVec = DEF_FORWARDVEC;
	m_speed = MIN_SPEED;
	m_brakeTimer = 0;
	m_driftParticleDelay = 0;
	m_rotX = 0.01f;
	m_rotX = 0;
	m_brakeBoostTimer = 0;
	m_shotTimer = 0;
	m_RedTime = 0;
	m_colorEasingTimer = 0;
	m_isRed = true;
	m_isEdge = false;
	m_isChangeRed = false;
	m_isChangeGreen = false;
	m_isBrake = false;
	m_isDamageEffectDrawPlayer = true;
	m_isDamageEffect = false;
	m_damageEffectTimer = 0;
	m_damageEffectCount = 0;
	m_hp = MAX_HP;

	m_model = Importer::Instance()->LoadModel("resource/user/", "player.glb");

	for (auto& index : m_driftParticle) {

		index = std::make_shared<DriftParticle>();

	}

}

void Player::Init()
{

	/*===== ���������� =====*/

	m_pos = Vec3<float>();
	m_inputVec = Vec3<float>();
	m_forwardVec = DEF_FORWARDVEC;
	m_prevForwardVec = DEF_FORWARDVEC;
	m_speed = MIN_SPEED;
	m_brakeTimer = 0;
	m_driftParticleDelay = 0;
	m_rotX = 0.01f;
	m_shotTimer = 0;
	m_RedTime = 0;
	m_colorEasingTimer = 0;
	m_isRed = true;
	m_brakeBoostTimer = 0;
	m_isEdge = false;
	m_isBrake = false;
	m_isChangeRed = false;
	m_isChangeGreen = false;
	m_isDamageEffectDrawPlayer = true;
	m_isDamageEffect = false;
	m_damageEffectTimer = 0;
	m_damageEffectCount = 0;
	m_hp = MAX_HP;

	for (auto& index : m_driftParticle) {

		index->Init();

	}

	m_outlineModel.Init(&m_pos, &m_rotation, 1.0f, 0.3f, Importer::Instance()->LoadModel("resource/user/", "playerOutline.glb"));

	//�_�b�V�����̃G�t�F�N�g
	dashLight.Init(&m_pos);


}

void Player::Update(Camera& Cam, std::weak_ptr<BulletMgr> BulletMgr, std::weak_ptr<EnemyMgr> EnemyMgr, const Vec2<float>& WindowSize, const float& MapSize, const float& EdgeScope)
{

	/*===== �X�V���� =====*/

	// ���͏���
	Input(Cam, WindowSize);

	// �ړ�����
	Move();

	// �e���ˏ���
	Shot(BulletMgr, EnemyMgr);

	// �����蔻�菈��
	CheckHit(BulletMgr, EnemyMgr, MapSize, EdgeScope);

	// �G�t�F�N�g�S�ʂ̍X�V����
	UpdateEffect();



}

void Player::Input(Camera& Cam, const Vec2<float>& WindowSize)
{

	/*====== ���͏��� =====*/

	// �X�e�B�b�N�̓��͂��擾�B
	Vec2<float> stickInput = UsersInput::Instance()->GetLeftStickVecRawFuna(0);

	// �}�E�X�̓��͂���x�N�g�������߂�B
	Vec2<float> screenPos = KuroFunc::ConvertWorldToScreen(m_pos, Cam.GetViewMat(), Cam.GetProjectionMat(), WindowSize);
	Vec2<float> mouseDir = Vec2<float>(UsersInput::Instance()->GetMousePos() - screenPos).GetNormal();
	m_inputVec = Vec3<float>(mouseDir.y, 0.0f, mouseDir.x);

	// �f�b�h���C����݂���B
	const float INPUT_DEADLINE = 0.25f;
	// ���͂���Ă�����B
	if (INPUT_DEADLINE < stickInput.Length()) {

		// ���͂�ۑ��B
		m_inputVec = Vec3<float>(stickInput.y, 0.0f, stickInput.x);

	}

	// ���ʃx�N�g����ۑ��B
	m_prevForwardVec = m_forwardVec;

	// �u���[�L���͂�ۑ��B
	m_isBrake = UsersInput::Instance()->ControllerInput(0, A) || UsersInput::Instance()->MouseInput(LEFT);
	if (m_isBrake) {

		++m_brakeTimer;

		// �u���[�L�u�[�X�g�̃^�C�}�[������������B
		m_brakeBoostTimer = 0;

		// �x�N�g�����������������⊮����B
		float cross = m_forwardVec.Cross(m_inputVec).y;
		float dot = m_forwardVec.Dot(m_inputVec);
		if (dot < 0.0f) {

			cross = 1.0f * (cross < 0 ? -1.0f : 1.0f);

		}
		float nowAngle = atan2f(m_forwardVec.x, m_forwardVec.z);
		if (cross != 0) {

			// �ۊǗ�
			float rot = 0.04f * cross;

			float rotAngle = nowAngle + rot;

			m_forwardVec = Vec3<float>(sinf(rotAngle), 0.0f, cosf(rotAngle));

		}

		GenerateDriftParticle(nowAngle, cross);

	}
	else {

		// �u���[�L�^�C�}�[��1�ȏゾ������u���[�L�𗣂����u�ԂƂ������ƁB
		if (0 < m_brakeTimer) {

			// �o�ߎ��Ԃ��犄�������߂�B
			float brakeRate = Saturate(static_cast<float>(m_brakeTimer) / static_cast<float>(MAX_BRAKE_TIMER));

			// �ړ����x�����߂�B
			m_speed = brakeRate * (MIN_SPEED + MAX_SPEED);

			// �u���[�L�u�[�X�g�̌��ʎ��Ԃ��v�Z����B
			if (0.5f < brakeRate) {
				m_brakeBoostTimer = brakeRate * MAX_BRAKE_TIMER;
			}

		}

		// �ő�l�A�ŏ��l�𒴂��Ȃ��悤�ɂ���B
		if (m_speed < MIN_SPEED) m_speed = MIN_SPEED;
		if (MAX_SPEED < m_speed) m_speed = MAX_SPEED;

		m_brakeTimer = 0;

	}


}

void Player::Move()
{

	/*===== �ړ����� =====*/

	// �u���[�L��Ԃ̗L���ɉ����Ĉړ����x��ς���B
	if (m_isBrake) {

		m_speed += (BRAKE_SPEED - m_speed) / 10.0f;

	}

	// ���W��ۑ��B
	m_prevPos = m_pos;

	// �ړ�������B
	m_pos += m_forwardVec * m_speed;

	// ���ʃx�N�g������]�����ʂ��v�Z����B
	if (UsersInput::Instance()->KeyInput(DIK_SPACE)) {

		int a = 0;

	}

	float forwardVecAngle = atan2f(m_forwardVec.x, m_forwardVec.z);
	float prevForwardVecAngle = atan2f(m_prevForwardVec.x, m_prevForwardVec.z);
	float subAngle = forwardVecAngle - prevForwardVecAngle;

	if (subAngle != 0) {

		m_rotX = -subAngle;

	}
	else {

		m_rotX -= m_rotX / 10.0f;

	}

}

void Player::UpdateEffect()
{

	/*===== �G�t�F�N�g�S�ʂ̍X�V���� =====*/

	// �h���t�g�p�[�e�B�N���̍X�V����
	for (auto& index : m_driftParticle) {

		if (!index->GetIsActive()) continue;

		index->Update(m_pos);

	}


	// �_���[�W�G�t�F�N�g�̍X�V����
	if (m_isDamageEffect) {

		++m_damageEffectTimer;
		if (DAMAGE_EFFECT_DRAW_CHANGE_SPAN <= m_damageEffectTimer) {

			// �v���C���[��\�����Ă��邩�ǂ����ŏ�����ς���B
			if (m_isDamageEffectDrawPlayer) {

				m_isDamageEffectDrawPlayer = false;

			}
			else {

				m_isDamageEffectDrawPlayer = true;

				++m_damageEffectCount;
				if (DAMAGE_EFFECT_COUNT <= m_damageEffectCount) {

					// �_���[�W�G�t�F�N�g���I����B
					m_isDamageEffect = false;

				}

			}

			m_damageEffectTimer = 0;

		}
	}



	// �f�o�b�O�p
	if (UsersInput::Instance()->KeyOnTrigger(DIK_O)) {

		float prevHP = m_hp;
		--m_hp;
		if (m_hp <= 0) m_hp = 0;
		if (m_hp <= 2 && 2 < prevHP) {
			m_isChangeRed = true;
			m_colorEasingTimer = 0;
		}

	}
	// �f�o�b�O�p
	if (UsersInput::Instance()->KeyOnTrigger(DIK_P)) {

		float prevHP = m_hp;
		++m_hp;

		if (3 <= m_hp && prevHP < 3) {

			m_isChangeGreen = true;

		}

		if (MAX_HP <= m_hp) {

			m_hp = MAX_HP;

		}

	}

	// �ԐF�ɕς���t���O���o���Ă�����B
	if (m_isChangeRed) {

		bool isEnd = ChangeBodyColorEasing(0.1f, Sine, In, GREEN_HSV, RED_HSV, true);

		// �C�[�W���O���I�������B
		if (isEnd) {

			m_isRed = true;
			m_isChangeRed = false;
			m_colorEasingTimer = 0.0f;

		}

	}

	// �ΐF�ɕς���t���O�������Ă�����B
	else if (m_isChangeGreen) {

		bool isEnd = ChangeBodyColorEasing(0.1f, Sine, In, RED_HSV, GREEN_HSV, true);

		if (isEnd) {

			m_isChangeGreen = false;
			m_colorEasingTimer = 0.0f;

		}

	}

	// HP��3�ȉ��������玩�@��_�ł�����B
	else if (m_hp <= 2) {

		// �Ԃ̎��B
		if (m_isRed) {

			// �Ԃł��鎞�Ԃ̃^�C�}�[���X�V����B
			++m_RedTime;
			if (RED_TIME[m_hp] <= m_RedTime) {

				// ��莞�Ԍo�߂��Ă���C�[�W���O�ŗ΂ɂ���B
				bool isEnd = ChangeBodyColorEasing(ADD_COLOR_EASING_TIMER[m_hp], Exp, In, RED_HSV, DARK_RED_HSV, false);

				if (isEnd) {

					m_isRed = false;
					m_colorEasingTimer = 0.0f;

				}

			}

		}
		else {

			// ��莞�Ԍo�߂��Ă���C�[�W���O�ňÂ��Ԃɂ���B
			bool isEnd = ChangeBodyColorEasing(ADD_COLOR_EASING_TIMER[m_hp], Exp, In, DARK_RED_HSV, RED_HSV, false);

			if (isEnd) {

				m_isRed = true;
				m_colorEasingTimer = 0.0f;
				m_RedTime = 0;

			}

		}

	}


	//�p���[�����܂�؂������o
	if (UsersInput::Instance()->KeyInput(DIK_A))
	{
		m_outlineModel.EnoughPowerEffect();
	}

	bool dashFlag = 1.0f <= m_brakeBoostTimer && !UsersInput::Instance()->MouseInput(LEFT);
	//�_�b�V�����̉��o
	if (dashFlag)
	{
		m_outlineModel.PowerUpEffect(50);
	}

	//�v���C���[�̃A�E�g���C���p
	m_outlineModel.Update();

	//�_�b�V�����̃G�t�F�N�g
	dashLight.Update(dashFlag);
}

#include"DrawFunc3D.h"
void Player::Draw(Camera& Cam) {

	/*===== �`�揈�� =====*/

	m_transform.SetPos(m_pos);

	// ���͂̊p�x�����߂�B
	Vec3<float> movedVec = m_forwardVec;
	//movedVec.Normalize();
	Vec2<float> inputVec = Vec2<float>(movedVec.x, movedVec.z);
	float inputAngle = atan2f(inputVec.x, inputVec.y);

	if (UsersInput::Instance()->KeyInput(DIK_O)) {

		m_rotX += 0.01f;

	}
	if (UsersInput::Instance()->KeyInput(DIK_P)) {

		m_rotX -= 0.01f;

	}

	// Y����]�̃N�H�[�^�j�I�����߂�B
	auto resultY = XMMatrixRotationQuaternion(XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 1), inputAngle));
	// X����]�̃N�H�[�^�j�I�����߂�B
	auto resultX = XMMatrixRotationQuaternion(XMQuaternionRotationAxis(XMVectorSet(m_forwardVec.x, m_forwardVec.y, m_forwardVec.z, 1.0f), m_rotX * 25.0f));

	// �N�H�[�^�j�I����������B
	m_rotation = resultY * resultX;

	m_transform.SetRotate(m_rotation);

	if (m_isDamageEffectDrawPlayer) {

		DrawFunc3D::DrawNonShadingModel(m_model, m_transform, Cam);
	}

	// �h���t�g�p�[�e�B�N���̕`�揈��
	for (auto& index : m_driftParticle) {

		if (!index->GetIsActive()) continue;

		index->Draw(Cam);

	}

	m_outlineModel.Draw(Cam);
	dashLight.Draw(Cam);

}

void Player::Shot(std::weak_ptr<BulletMgr> BulletMgr, std::weak_ptr<EnemyMgr> EnemyMgr) {

	/*===== �e�������� =====*/

	++m_shotTimer;
	if (SHOT_TIMER <= m_shotTimer) {

		m_shotTimer = 0;

		// ��ԋ߂��ɂ���G����������B
		Vec3<float> nearestEnemy = EnemyMgr.lock()->SearchNearestEnemy(m_pos);

		// Vec3<float>(-1,-1,-1)��������G�����Ȃ��̂Ō����Ȃ��B
		if (nearestEnemy == Vec3<float>(-1, -1, -1)) return;

		// �������U������B
		Vec3<float> shotEnemyPos = nearestEnemy;
		shotEnemyPos.x += KuroFunc::GetRand(-3.0f, 3.0f);
		shotEnemyPos.z += KuroFunc::GetRand(-3.0f, 3.0f);

		// �G�̕����Ɍ������Ēe�����B
		BulletMgr.lock()->GeneratePlayerBullet(m_pos, (shotEnemyPos - m_pos).GetNormal());

	}

}

void Player::DrawDebugInfo(Camera& Cam) {

	/*===== �f�o�b�O����`�� =====*/

	if (m_isBrake) {

		Vec2<float> inputVec = Vec2<float>(m_inputVec.x, m_inputVec.z);
		float brakeRate = 0;

		// �o�ߎ��Ԃ��犄�������߂�B
		brakeRate = Saturate(static_cast<float>(m_brakeTimer) / static_cast<float>(MAX_BRAKE_TIMER)); // 0.5f ~ 1.5f �͈̔�


		DrawFunc3D::DrawLine(Cam, m_pos, m_pos + Vec3<float>(inputVec.x, 0.0f, inputVec.y).GetNormal() * (brakeRate * 20.0f), Color(1.0f, 0.0f, 0.0f, 1.0f), 1.0f);

	}

}

void Player::CheckHit(std::weak_ptr<BulletMgr> BulletMgr, std::weak_ptr<EnemyMgr> EnemyMgr, const float& MapSize, const float& EdgeScope)
{

	/*===== �����蔻�� =====*/

	// �}�b�v�Ƃ̓����蔻��B
	m_pos = KeepInMap(m_pos, MapSize);

	// �G�b�W�̔���B
	m_isEdge = MapSize - m_pos.Length() < EdgeScope;

	// �G�e�Ƃ̓����蔻��B
	int hitCount = BulletMgr.lock()->CheckHitEnemyBullet(m_pos, SCALE);

	// �G�Ƃ̃G�b�W�̔���B
	if (EnemyMgr.lock()->CheckEnemyEdge(m_pos, SCALE)) {

		m_isEdge = true;

	}

	// �u�[�X�g�ʂ����ȏゾ�����炠����x�͈̔͂̓G��|���B
	--m_brakeBoostTimer;
	if (0 < m_brakeBoostTimer) {

		EnemyMgr.lock()->AttackEnemy(m_pos, BOOST_SCALE);

	}
	else {

		m_brakeBoostTimer = 0;

	}

	// �G�Ƃ̓����蔻��B
	if (EnemyMgr.lock()->CheckHitEnemy(m_pos, SCALE)) {

		// ������������B
		m_damageEffectTimer = 0;
		m_damageEffectCount = 0;
		m_isDamageEffect = true;
		float prevHP = m_hp;
		--m_hp;
		if (m_hp <= 0) m_hp = 0;
		if (m_hp <= 2 && 2 < prevHP) {
			m_isChangeRed = true;
			m_colorEasingTimer = 0;
		}

	}

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

void Player::GenerateDriftParticle(const float& NowAngle, const float& Cross) {

	++m_driftParticleDelay;
	if (DRIFT_PARTICLE_DELAY < m_driftParticleDelay) {

		// �h���t�g�p�[�e�B�N���𐶐�
		Vec3<float> generatePos;
		int generateCount = 0;
		const float MAX_GENERATE_COUNT = 3.0f;
		for (auto& index : m_driftParticle) {

			if (index->GetIsActive()) continue;

			// �O�ς̌��ʂ��}�C�i�X(��)��������
			float shotAngle = NowAngle;
			if (Cross < 0) {

				shotAngle += DirectX::XM_PI;

			}
			else {

				shotAngle -= DirectX::XM_PI;

			}

			// �����ꏊ�����߂�B
			if (static_cast<int>(&index - &m_driftParticle[0]) % 2 == 0) {

				// �O�֕���
				generatePos = m_pos + m_forwardVec * 3.0f;

			}
			else {

				// ��֕���
				generatePos = m_pos - m_forwardVec * 2.5f;

			}

			index->Generate(generatePos, shotAngle, Cross);

			++generateCount;

			// �h���t�g�ŏ���F��������B
			if (m_brakeTimer <= 15) {

				if (10 < generateCount) {

					break;

				}

			}
			else {

				if (MAX_GENERATE_COUNT * fabs(Cross) < generateCount) {

					break;

				}

			}

		}

		m_driftParticleDelay = 0;

	}


}

Vec3<float> Player::RGBtoHSV(const Vec3<float>& RGB)
{
	double hue = 0;
	double s = 0;
	double value = 0;
	double max = 0;
	double min = 0;
	int colorIndex = 0;
	Vec3<float> rgb = RGB;
	SearchMaxMinColor(rgb, max, min, colorIndex);

	/*RGB -> HSV*/
	//�F�������߂�
	//R���ő�l�̏ꍇ
	if (colorIndex == 0) {
		hue = 60.0f * ((rgb.y - rgb.z) / (max - min));
	}
	//G���ő�l�̏ꍇ
	if (colorIndex == 1) {
		hue = 60.0f * (rgb.z - rgb.x) / (max - min) + 120;
	}
	//B���ő�l�̏ꍇ
	if (colorIndex == 2) {
		hue = 60.0f * (rgb.x - rgb.y) / (max - min) + 240;
	}
	//���ׂĂ̐F�������������ꍇ
	if (colorIndex == 3) {
		hue = 0;
	}
	//�F�����}�C�i�X�l�������ꍇ��360�𑫂�
	if (hue < 0) {
		hue += 360;
	}

	//�ʓx�����߂�
	s = (max - min) / max * 255.0f;

	//���x�����߂�
	value = max;

	return Vec3<float>(hue, s, value);

}

Vec3<float> Player::HSVtoRGB(const Vec3<float>& HSV)
{
	/*HSV -> RGB*/

	//�ő�l�ƍŏ��l�����߂�
	float max = HSV.z;
	float min = max - ((HSV.y / 255.0f) * max);

	//RGB�����߂�
	float hue = HSV.x;
	Vec3<float> rgb;
	if (hue >= 0 && hue < 60) {
		rgb.x = max;
		rgb.y = (hue / 60) * (max - min) + min;
		rgb.z = min;
	}
	else if (hue >= 60 && hue < 120) {
		rgb.x = ((120 - hue) / 60) * (max - min) + min;
		rgb.y = max;
		rgb.z = min;
	}
	else if (hue >= 120 && hue < 180) {
		rgb.x = min;
		rgb.y = max;
		rgb.z = ((hue - 120) / 60) * (max - min) + min;
	}
	else if (hue >= 180 && hue < 240) {
		rgb.x = min;
		rgb.y = ((240 - hue) / 60) * (max - min) + min;
		rgb.z = max;
	}
	else if (hue >= 240 && hue < 300) {
		rgb.x = ((hue - 240) / 60) * (max - min) + min;
		rgb.y = min;
		rgb.z = max;
	}
	else if (hue >= 300 && hue <= 360) {
		rgb.x = max;
		rgb.y = min;
		rgb.z = ((360 - hue) / 60) * (max - min) + min;
	}

	rgb = Vec3<float>(rgb.x, rgb.y, rgb.z);

	return rgb;
}

void Player::SearchMaxMinColor(Vec3<float>& Color, double& max, double& min, int& rgb)
{

	float r = Color.x;
	float g = Color.y;
	float b = Color.z;
	max = r;
	min = r;
	rgb = 0;

	//���ׂĂ̐F�������l�̏ꍇ
	if (r == g && g == b) {
		max = r;
		min = r;
		rgb = 3;
		return;
	}

	//�ő�l�����߂�
	if (max < g) {
		max = g;
		rgb = 1;
	}
	if (max < b) {
		max = b;
		rgb = 2;
	};
	//�ŏ��l�����߂�
	if (min > g) {
		min = g;
	}
	if (min > b) {
		min = b;
	}

}

bool Player::ChangeBodyColorEasing(const float& AddEasingAmount, EASING_TYPE EasingType, EASE_CHANGE_TYPE EaseChangeType, const Vec3<float>& StartColor, const Vec3<float>& EndColor, const bool& IsEaseX)
{

	/*===== �F���C�[�W���O�ŕς��� =====*/

	m_colorEasingTimer += AddEasingAmount;
	if (1.0f <= m_colorEasingTimer) {

		m_colorEasingTimer = 1.0f;

	}

	// �C�[�W���O�ʂ��v�Z����B
	float easingAmount = KuroMath::Ease(EaseChangeType, EasingType, m_colorEasingTimer, 0.0f, 1.0f);

	// �F���v�Z����B
	Vec3<float> nowHSV = StartColor;
	if (IsEaseX) {
		nowHSV.x += Vec3<float>(EndColor - StartColor).x * easingAmount;
	}
	else {
		nowHSV.z += Vec3<float>(EndColor - StartColor).z * easingAmount;
	}
	if (nowHSV.x < 0) {
		nowHSV.x += 360.0f;
	}
	m_model->m_meshes[1].material->constData.pbr.baseColor = HSVtoRGB(nowHSV);
	m_model->m_meshes[1].material->Mapping();

	return 1.0f <= m_colorEasingTimer;

}