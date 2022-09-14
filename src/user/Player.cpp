#include "Player.h"
#include "../engine/UsersInput.h"
#include "Importer.h"
#include "Model.h"
#include "BulletMgr.h"
#include "EnemyMgr.h"
#include "KuroFunc.h"
#include "GameManager.h"
#include "DriftParticle.h"
#include "KazCollisionHelper.h"
#include "PlayerHP.h"
#include"ModelAnimator.h"
#include "SlowMgr.h"
#include"AudioApp.h"
#include"PlayerDamageEffect.h"
#include "PlayerArrow.h"

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
	m_movedLength = 0;
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
	m_dashCounter = 0;
	m_hp = MAX_HP;
	m_isFever = false;
	m_isPrevFever = false;
	m_feverTime = 0;
	m_mousePointerScale = false;

	m_knockBackVec = Vec3<float>();
	m_knockBackSpeed = 0;

	m_modelObj = std::make_shared<ModelObject>("resource/user/", "player.glb");
	m_mousePointer = std::make_shared<ModelObject>("resource/user/", "mousePoint3D.glb");

	materialColor = m_modelObj->m_model->m_meshes[0].material->constData.pbr.baseColor;

	m_playerArrow = std::make_shared<PlayerArrow>();

	for (auto& index : m_driftParticle) {

		index = std::make_shared<DriftParticle>();

	}

	float angleInterval = DirectX::XM_PI / 10.0f;
	for (auto& index : m_hpUI) {

		int indexCount = static_cast<int>(&index - &m_hpUI[0]);

		// ��]��
		float drawAngle = 0;

		// �C���f�b�N�X��10�ȏゾ������B
		if (10 < indexCount) {

			drawAngle = DirectX::XM_PIDIV2 - angleInterval * GetDigits(indexCount, 0, 0);

		}
		else {

			drawAngle = DirectX::XM_PIDIV2 - angleInterval * static_cast<float>(indexCount);

		}

		index = std::make_shared<PlayerHP>(-drawAngle);

	}

	m_dashLight = std::make_unique<PlayerDashLighting>();

	m_damageEffect = std::make_shared<PlayerDamageEffectMgr>();

	m_damageSE = AudioApp::Instance()->LoadAudio("resource/user/sound/damage.wav");
	m_dashSE = AudioApp::Instance()->LoadAudio("resource/user/sound/dash.wav", 0.4f);
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
	m_movedLength = 0;
	m_dashCounter = 0;
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
	m_isFever = false;
	m_isPrevFever = false;
	m_feverTime = 0;
	m_mousePointerScale = 0;

	m_damageEffect->Init();

	m_knockBackVec = Vec3<float>();
	m_knockBackSpeed = 0;

	for (auto& index : m_driftParticle) {

		index->Init();

	}

	for (auto& index : m_hpUI) {

		index->Init();

	}

	m_outlineModel.Init(&m_pos, &m_rotation, 1.0f, 0.3f, Importer::Instance()->LoadModel("resource/user/", "playerOutline.glb"));

	//�_�b�V�����̃G�t�F�N�g
	m_dashLight->Init(&m_pos);

	m_modelObj->m_animator->Play("ToFloater", false, false);

	m_firstInput = false;
}

void Player::Update(Camera& Cam, std::weak_ptr<BulletMgr> BulletMgr, std::weak_ptr<EnemyMgr> EnemyMgr, const Vec2<float>& WindowSize, const float& MapSize, const float& EdgeScope, bool IsStopFeverTimer, bool IsNoInput)
{

	/*===== �X�V���� =====*/

	// ���͏���
	if (!IsNoInput) {
		Input(Cam, WindowSize);
	}

	// �ړ�����
	Move(BulletMgr, IsStopFeverTimer);

	// �e���ˏ���
	Shot(BulletMgr, EnemyMgr);

	// �����蔻�菈��
	CheckHit(BulletMgr, EnemyMgr, MapSize, EdgeScope);

	// �G�t�F�N�g�S�ʂ̍X�V����
	UpdateEffect();

	// HPUI�̍X�V����
	for (auto& index : m_hpUI) {

		index->Invalidate();

	}
	if (10 < m_hp) {

		for (int index = 0; index < GetDigits(m_hp, 0, 0); ++index) {

			m_hpUI[index]->Activate();

		}

	}
	else {

		for (int index = 0; index < m_hp; ++index) {

			m_hpUI[index]->Activate();

		}

	}

	// HPUI�̍X�V����
	for (auto& index : m_hpUI) {

		index->Update(m_pos, SCALE);

	}

	//�A�j���[�^�[�X�V
	m_modelObj->m_animator->Update();
}

void Player::Input(Camera& Cam, const Vec2<float>& WindowSize)
{

	/*====== ���͏��� =====*/

	// �}�E�X�̓��͂���x�N�g�������߂�B
	Vec2<float> screenPos = KuroFunc::ConvertWorldToScreen(m_pos, Cam.GetViewMat(), Cam.GetProjectionMat(), WindowSize);
	Vec2<float> mouseDir = Vec2<float>(UsersInput::Instance()->GetMousePos() - screenPos).GetNormal();
	m_inputVec = Vec3<float>(mouseDir.y, 0.0f, mouseDir.x);

	// ���ʃx�N�g����ۑ��B
	m_prevForwardVec = m_forwardVec;

	// �X�N���[�����W�n�ł̐��ʃx�N�g���B
	float screenForwardAngle = atan2f(m_forwardVec.x, m_forwardVec.z);
	Vec2<float> screenForwardVec = Vec2<float>(cosf(screenForwardAngle), sinf(screenForwardAngle));

	// �u���[�L���͂�ۑ��B
	bool oldBrake = m_isBrake;
	m_isBrake = UsersInput::Instance()->MouseInput(LEFT);
	if (!m_firstInput)m_firstInput = true;

	//�^�C���A�j���[�V����
	if (!oldBrake && m_isBrake)	//�t���[�^�[ �� �^�C��
	{
		m_modelObj->m_animator->Play("ToWheel", false, false);
	}
	else if (oldBrake && !m_isBrake)	//�^�C�� �� �t���[�^�[
	{
		m_modelObj->m_animator->Play("ToFloater", false, false);
	}

	if (m_isBrake) {

		++m_brakeTimer;

		// �u���[�L�u�[�X�g�̃^�C�}�[������������B
		m_brakeBoostTimer = 0;

		// �}�E�X�̂�[��Ǎ��W�����߂�B
		Vec2<int> winSize = Vec2<int>(WindowSize.x, WindowSize.y);
		Vec3<float> mousePos = KuroFunc::ConvertScreenToWorld(UsersInput::Instance()->GetMousePos(), 0, Cam.GetViewMat(), Cam.GetProjectionMat(), winSize);
		Vec3<float> mousePosFar = KuroFunc::ConvertScreenToWorld(UsersInput::Instance()->GetMousePos(), 1, Cam.GetViewMat(), Cam.GetProjectionMat(), winSize);
		Vec3<float> mouseWorldPos;
		bool isHit = CheckHitRayToStagePolygon(mousePos, Vec3<float>(mousePosFar - mousePos).GetNormal(), mouseWorldPos);

		// �}�E�X���W���X�V�B
		mouseWorldPos.y = 0;
		m_mousePointer->m_transform.SetPos(mouseWorldPos);

		// �x�N�g�����������������⊮����B
		float cross = m_forwardVec.Cross(m_inputVec).y;
		float dot = m_forwardVec.Dot(m_inputVec);
		if (dot < 0.0f) {

			cross = 1.0f * (cross < 0 ? -1.0f : 1.0f);

		}
		float nowAngle = atan2f(m_forwardVec.x, m_forwardVec.z);
		if (cross != 0) {

			// �ۊǗ�
			float rot = 0.04f * cross * SlowMgr::Instance()->m_slow;

			float rotAngle = nowAngle + rot;

			m_forwardVec = Vec3<float>(sinf(rotAngle), 0.0f, cosf(rotAngle));

		}

		// �}�E�X�J�[�\���̃T�C�Y��⊮����B
		m_mousePointerScale += (MOUSE_POINTER_SCALE - m_mousePointerScale) / 2.0f;

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

			// �_�b�V�������񐔂��J�E���g����B
			m_dashCounter += 1.0f * SlowMgr::Instance()->m_slow;

			AudioApp::Instance()->PlayWaveDelay(m_dashSE,4);
		}

		// �ő�l�A�ŏ��l�𒴂��Ȃ��悤�ɂ���B
		if (m_speed < MIN_SPEED) m_speed = MIN_SPEED;
		if (MAX_SPEED < m_speed) m_speed = MAX_SPEED;

		m_brakeTimer = 0;


		// �}�E�X�J�[�\���̃T�C�Y��⊮����B
		if (0 < m_mousePointerScale) {
			m_mousePointerScale -= m_mousePointerScale / 2.0f;
		}

	}


}

void Player::Move(std::weak_ptr<BulletMgr> BulletMgr, bool IsStopFeverTimer)
{
	if (!m_firstInput)return;

	/*===== �ړ����� =====*/

	m_isPrevFever = m_isFever;

	if (BulletMgr.lock()->GetIsKillElecMushi()) {

		m_isFever = true;
		m_feverTime = FEVER_TIME;
		BulletMgr.lock()->BrakeIsKillElecMushi();

	}

	// �m�b�N�o�b�N�̈ړ��ʂ�ݒ�B
	if (0 < m_knockBackSpeed) {

		// �ړ�������B
		m_pos += m_knockBackVec * m_knockBackSpeed;

		// �ړ��ʂ�0�ɋ߂Â���B
		m_knockBackSpeed -= m_knockBackSpeed / 18.0f;

	}

	// �t�B�[�o�[��Ԃ�������
	if (m_isFever) {

		// �t�B�[�o�[�̎��Ԍo�߂��~�߂�t���O�������Ă����猸�炳�Ȃ��B�`���[�g���A���Ńt�B�[�o�[�̃^�C�}�[�����炵�����Ȃ��ꍇ���������̂ō쐬���܂����B
		if (!IsStopFeverTimer) {

			m_feverTime -= 1.0f * SlowMgr::Instance()->m_slow;
			if (m_feverTime <= 0) {

				m_isFever = false;

			}

		}

		m_speed += (MAX_SPEED - m_speed) / 2.0f * SlowMgr::Instance()->m_slow;

	}
	else {

		// �u���[�L��Ԃ̗L���ɉ����Ĉړ����x��ς���B
		if (m_isBrake) {

			m_speed += (BRAKE_SPEED - m_speed) / 60.0f * SlowMgr::Instance()->m_slow;

		}

	}

	// ���W��ۑ��B
	m_prevPos = m_pos;

	// �ړ�������B
	m_pos += m_forwardVec * m_speed * SlowMgr::Instance()->m_slow;
	m_movedLength += Vec3<float>(m_forwardVec * m_speed * SlowMgr::Instance()->m_slow).Length();

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

		m_damageEffectTimer += 1.0f * SlowMgr::Instance()->m_slow;
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

		// �A�E�g���C���̐F��Ԃɂ���B
		m_outlineModel.ChangeColor(ColorPalette::S_RED);

		// ���@�̐F��Ԃɂ���B	
		m_modelObj->m_model->m_meshes[0].material->constData.pbr.baseColor = Vec3<float>(ColorPalette::S_RED.m_r, ColorPalette::S_RED.m_g, ColorPalette::S_RED.m_b);
		m_modelObj->m_model->m_meshes[0].material->Mapping();

		// ���@�̐F��Ԃɂ���B	
		m_modelObj->m_model->m_meshes[1].material->constData.pbr.baseColor = Vec3<float>(ColorPalette::S_RED.m_r, ColorPalette::S_RED.m_g, ColorPalette::S_RED.m_b);
		m_modelObj->m_model->m_meshes[1].material->Mapping();

	}
	else {

		// �A�E�g���C���̐F��΂ɂ���B
		m_outlineModel.ChangeColor(ColorPalette::S_GREEN_COLOR);

		// ���@�̐F�����ɖ߂��B	
		m_modelObj->m_model->m_meshes[0].material->constData.pbr.baseColor = materialColor;
		m_modelObj->m_model->m_meshes[0].material->Mapping();

		// ���@�̐F��΂ɂ���B	
		m_modelObj->m_model->m_meshes[1].material->constData.pbr.baseColor = Vec3<float>(ColorPalette::S_GREEN_COLOR.m_r, ColorPalette::S_GREEN_COLOR.m_g, ColorPalette::S_GREEN_COLOR.m_b);
		m_modelObj->m_model->m_meshes[1].material->Mapping();

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


	//�p���[�����܂�؂������o
	if (UsersInput::Instance()->KeyInput(DIK_A))
	{
		m_outlineModel.EnoughPowerEffect();
	}

	bool dashFlag = 1.0f <= m_brakeBoostTimer;
	//�_�b�V�����̉��o
	if (dashFlag)
	{
		m_outlineModel.PowerUpEffect(50);
	}

	//�v���C���[�̃A�E�g���C���p
	m_outlineModel.Update();

	//�_�b�V�����̃G�t�F�N�g
	m_dashLight->Update(dashFlag, m_isFever);

	// �_���[�W�G�t�F�N�g���X�V�B
	m_damageEffect->Update();

}

#include"DrawFunc_Append.h"
void Player::Draw(Camera& Cam, const bool& IsTitle)
{
	/*===== �`�揈�� =====*/

	m_modelObj->m_transform.SetPos(m_pos);

	// ���͂̊p�x�����߂�B
	Vec3<float> movedVec = m_forwardVec;
	//movedVec.Normalize();
	Vec2<float> inputVec = Vec2<float>(movedVec.x, movedVec.z);
	float inputAngle = atan2f(inputVec.x, inputVec.y);

	// Y����]�̃N�H�[�^�j�I�����߂�B
	auto resultY = XMMatrixRotationQuaternion(XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 1), inputAngle));
	// X����]�̃N�H�[�^�j�I�����߂�B
	auto resultX = XMMatrixRotationQuaternion(XMQuaternionRotationAxis(XMVectorSet(m_forwardVec.x, m_forwardVec.y, m_forwardVec.z, 1.0f), m_rotX * 25.0f));

	// �N�H�[�^�j�I����������B
	m_rotation = resultY * resultX;

	inputATan2f = atan2f(m_inputVec.x, m_inputVec.z);
	m_modelObj->m_transform.SetRotate(m_rotation);

	if (m_isDamageEffectDrawPlayer) {

		DrawFunc_Append::DrawModel(m_modelObj);

	}

	// �}�E�X�̃J�[�\����`��
	m_mousePointer->m_transform.SetScale(m_mousePointerScale);
	DrawFunc_Append::DrawModel(m_mousePointer);

	// �h���t�g�p�[�e�B�N���̕`�揈��
	for (auto& index : m_driftParticle) {

		if (!index->GetIsActive()) continue;

		index->Draw();

	}

	if (!IsTitle) {

		// �A�E�g���C����`��
		if (!m_isDamageEffect) {
			m_outlineModel.Draw(Cam);
		}

		m_dashLight->Draw(Cam);

		// �G��HP�̔|����`��
		for (auto& index : m_hpUI) {

			index->Draw();

		}

	}

	// �_���[�W���̃G�t�F�N�g��`��B
	m_damageEffect->Draw();

	m_playerArrow->Draw(m_pos, m_mousePointer->m_transform.GetPos());

}

void Player::Shot(std::weak_ptr<BulletMgr> BulletMgr, std::weak_ptr<EnemyMgr> EnemyMgr) {

	/*===== �e�������� =====*/

	// �h���t�g���Ă��Ȃ������珈���Ƃ��΂��B
	if (!m_isBrake) return;

	m_shotTimer += 1.0f * SlowMgr::Instance()->m_slow;
	if (SHOT_TIMER <= m_shotTimer) {

		m_shotTimer = 0;

		// ��ԋ߂��ɂ���G����������B
		Vec3<float> nearestEnemy = EnemyMgr.lock()->SearchNearestEnemyToVector(m_pos, m_inputVec, 0.8f);

		Vec3<float> shotEnemyPos = m_pos + m_inputVec * 20.0f;
		const float AUTO_AIM_SCALE = 60.0f;
		if (nearestEnemy != Vec3<float>(-1, -1, -1) && Vec3<float>(nearestEnemy - m_pos).Length() <= AUTO_AIM_SCALE) {

			shotEnemyPos = nearestEnemy;

		}

		// �������U������B
		shotEnemyPos.x += KuroFunc::GetRand(-2.0f, 2.0f);
		shotEnemyPos.z += KuroFunc::GetRand(-2.0f, 2.0f);

		// �G�̕����Ɍ������Ēe�����B
		BulletMgr.lock()->GeneratePlayerBullet(m_pos, (shotEnemyPos - m_pos).GetNormal());

	}

}

#include"DrawFunc3D.h"
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

#include"ShakeMgr.h"
#include"EnemyWaveEditor.h"
void Player::CheckHit(std::weak_ptr<BulletMgr> BulletMgr, std::weak_ptr<EnemyMgr> EnemyMgr, const float& MapSize, const float& EdgeScope)
{

	/*===== �����蔻�� =====*/

	// �}�b�v�Ƃ̓����蔻��B
	bool isHitMap = false;
	m_pos = KazCollisionHelper::KeepInMap(m_pos, MapSize);

	// �G�b�W�̔���B
	m_isEdge = MapSize - m_pos.Length() < EdgeScope;

	//���x���f�U�C����
	if (!EnemyWaveEditor::Instance()->CanWaveUpdate())return;

	// �_���[�W�G�t�F�N�g���͓����蔻��𖳌�������B
	if (m_isDamageEffect) return;

	// �ǍۂƂ̓����蔻����s���B
	if (MapSize - MAP_EDGE <= m_pos.Length()) {

		Damage();

		// �m�b�N�o�b�N�̈ړ��ʂ�ݒ�B
		m_knockBackVec = -m_pos.GetNormal();
		m_knockBackSpeed = KNOCK_BACK_SPEED;
		m_speed = 0.0f;

	}

	// �G�e�Ƃ̓����蔻��B
	int hitCount = BulletMgr.lock()->CheckHitEnemyBullet(m_pos, SCALE);
	if (0 < hitCount) {
		// ������������B
		Damage();
	}

	// �u�[�X�g�ʂ����ȏゾ�����炠����x�͈̔͂̓G��|���B
	m_brakeBoostTimer -= 1.0f * SlowMgr::Instance()->m_slow;
	if (0 < m_brakeBoostTimer) {

		//EnemyMgr.lock()->AttackEnemy(m_pos, BOOST_SCALE, BulletMgr);

	}
	else {

		m_brakeBoostTimer = 0;

	}

	// �t�B�[�o�[��Ԃ�������
	if (m_isFever) {

		int hitCount = EnemyMgr.lock()->AttackEnemy(m_pos, BOOST_SCALE, BulletMgr);

		if (0 < hitCount) {

			// �V�F�C�N��������B
			ShakeMgr::Instance()->Shake(5);

		}

	}

	// �G�Ƃ̓����蔻��B
	if (EnemyMgr.lock()->CheckHitEnemy(m_pos, SCALE)) {
		Damage();
	}

}


void Player::Damage()
{
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

	AudioApp::Instance()->PlayWaveDelay(m_damageSE);

	m_damageEffect->Start();

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
	m_modelObj->m_model->m_meshes[1].material->constData.pbr.baseColor = ColorPalette::HSVtoRGB(nowHSV);
	m_modelObj->m_model->m_meshes[1].material->Mapping();

	// �A�E�g���C���̐F��ς���B
	Color outlineColor;
	outlineColor.m_r = m_modelObj->m_model->m_meshes[1].material->constData.pbr.baseColor.x;
	outlineColor.m_g = m_modelObj->m_model->m_meshes[1].material->constData.pbr.baseColor.y;
	outlineColor.m_b = m_modelObj->m_model->m_meshes[1].material->constData.pbr.baseColor.z;
	outlineColor.m_a = 1.0f;
	m_outlineModel.ChangeColor(outlineColor);

	return 1.0f <= m_colorEasingTimer;

}