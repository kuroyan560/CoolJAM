#include"TutorialScene.h"
#include"GameManager.h"
#include"DrawFunc3D.h"
#include"Player.h"
#include"Importer.h"
#include"DrawFunc2D.h"
#include"EnemyMgr.h"
#include"BulletMgr.h"
#include"WinApp.h"
#include"EnvironmentMgr.h"
#include"EnemyWaveMgr.h"
#include"GameTimer.h"
#include"DrawFunc_Append.h"
#include"../engine/Common/Angle.h"
#include"KazCollisionHelper.h"
#include"../engine/DrawFuncBillBoard.h"
#include"FeverGauge.h"
#include"GameMode.h"
#include"Tutorial.h"
#include"StageFloor.h"
#include"SlowMgr.h"
#include"ShakeMgr.h"

TutorialScene::TutorialScene()
{
	//�o�b�N�o�b�t�@���擾
	auto backBuff = D3D12App::Instance()->GetBackBuffRenderTarget();

	/*===== �R���X�g���N�^ =====*/

	//�f�v�X�X�e���V�������i�o�b�N�o�b�t�@�Ɠ����T�C�Y�j
	m_depthStencil = D3D12App::Instance()->GenerateDepthStencil(backBuff->GetGraphSize());

	m_player = std::make_shared<Player>();
	m_player->Init();

	m_gameCam = std::make_shared<Camera>(m_gameCamKey);
	GameManager::Instance()->RegisterCamera(m_gameCamKey, m_gameCam);
	GameManager::Instance()->ChangeCamera(m_gameCamKey);

	//�G�𐶐��B
	m_enemyMgr = std::make_shared<EnemyMgr>();
	m_enemyMgr->Init();

	//�e�N���X�𐶐��B
	m_bulletMgr = std::make_shared<BulletMgr>();

	// �t�B�[�o�[�Q�[�W
	m_feverGauge = std::make_unique<FeverGauge>();

	// �V�[���J��
	m_sceneTransition = std::make_unique<SceneTransition>();

	//�G�E�F�[�u�Ǘ��N���X
	m_enemyWaveMgr = std::make_unique<EnemyWaveMgr>(MAP_SIZE);

	//�G�~�b�V�u�}�b�v����
	m_emissiveMap = D3D12App::Instance()->GenerateRenderTarget(DXGI_FORMAT_R32G32B32A32_FLOAT, Color(0, 0, 0, 1), backBuff->GetGraphSize(), L"EmissiveMap");

	//�f�v�X�}�b�v����
	m_depthMap = D3D12App::Instance()->GenerateRenderTarget(DXGI_FORMAT_R32_FLOAT, Color(0, 0, 0, 0), backBuff->GetGraphSize(), L"DepthMap");

	//���}�l�[�W������
	m_environmentMgr = std::make_unique<EnvironmentMgr>();

	const auto backBuffFormat = backBuff->GetDesc().Format;
	DrawFunc_Append::RegisterRenderTargets(backBuffFormat, m_emissiveMap, m_depthMap, m_depthStencil);
	DrawFunc3D::GenerateDrawLinePipeline(backBuffFormat);
	DrawFunc3D::GenerateDrawLinePipeline(backBuffFormat, AlphaBlendMode_Add);

	m_isCameraHomePosition = true;
	m_isTransitionStart = false;
	m_isInitPlayer = false;
	m_isCompleteUpper = false;
	m_returnTitlePosEasingTimer = 0;
	m_transitionDelayTimer = 0;

	m_tutorial = std::make_shared<Tutorial>();

	m_isFeverCameraEffect = false;
	m_feverNearCameraTimer = 0;

	ShakeMgr::Instance()->Init();


	SlowMgr::Instance()->Init();
}

void TutorialScene::OnInitialize()
{

	/*===== ���������� =====*/
	m_player->Init();
	m_enemyMgr->Init();
	m_bulletMgr->Init();
	m_enemyWaveMgr->Init(60);
	m_feverGauge->Init();

	m_environmentMgr->Init();
	m_player->Init();

	m_isCameraHomePosition = true;
	m_isTransitionStart = false;
	m_isInitPlayer = false;
	m_isCompleteUpper = false;
	m_returnTitlePosEasingTimer = 0;
	m_transitionDelayTimer = 0;

	m_tutorial->Init();

	ShakeMgr::Instance()->Init();

	if (GameMode::Instance()->m_id == GameMode::ID::GAME) {

		const float CAMERA_DISTANCE = 80.0f;
		m_baseEye = m_player->GetPos() + Vec3<float>(CAMERA_DISTANCE, CAMERA_DISTANCE, 0);
		m_nowEye = m_baseEye;
		m_baseTarget = m_player->GetPos();
		m_nowTarget = m_baseTarget;

	}
	else {

		m_baseEye = CAMERA_HOME_EYE_POSITION;
		m_nowEye = CAMERA_HOME_EYE_POSITION;
		m_baseTarget = CAMERA_HOME_TARGET_POSITION;
		m_nowTarget = CAMERA_HOME_TARGET_POSITION;

	}

	SlowMgr::Instance()->Init();

	m_isFeverCameraEffect = false;
	m_feverNearCameraTimer = 0;

	DrawFunc_Append::RegisterRenderTargets(D3D12App::Instance()->GetBackBuffFormat(), m_emissiveMap, m_depthMap, m_depthStencil);
}

void TutorialScene::OnUpdate()
{
	/*===== �X�V���� =====*/

	//���݂̃J�����擾
	auto& nowCam = *GameManager::Instance()->GetNowCamera();



	//�X�N���[���T�C�Y���擾�B
	Vec2<float> windowSize = Vec2<float>(WinApp::Instance()->GetWinSize().x, WinApp::Instance()->GetWinSize().y);


	//�Q�[���}�l�[�W���X�V
	GameManager::Instance()->Update();

	// �v���C���[�X�V����
	m_player->Update(nowCam, m_bulletMgr, m_enemyMgr, windowSize, MAP_SIZE, EDGE_SCOPE, m_tutorial->GetPlayerFeverGaugeStop(m_enemyMgr));

	// �J�ڂ��n�܂�����v���C���[����ɏ�������������B
	if (m_isInitPlayer) {

		m_player->Init();

		m_tutorial->Init();
	}

	// ���̐F��ς���B
	if (m_player->GetIsFever())
	{
		m_environmentMgr->ChangeColor(m_enemyMgr->GetParticleColor());
	}
	else
	{
		m_environmentMgr->ChangeColor(Color(0.0f, 0.0f, 0.0f, 1.0f));
	}

	if (m_player->GetIsFever())
	{
		m_environmentMgr->ChangeColor(m_enemyMgr->GetParticleColor());
	}
	else
	{
		m_environmentMgr->ChangeColor(Color(0.0f, 0.0f, 0.0f, 1.0f));
	}


	// �G�X�V����
	m_enemyMgr->Update(m_bulletMgr, m_player->GetPos(), MAP_SIZE);

	// �e���X�V�B
	m_bulletMgr->Update(MAP_SIZE);

	// �`���[�g���A���̃A�b�v�f�[�g
	if (0.9f <= SlowMgr::Instance()->m_slow) {
		m_tutorial->Update(m_player, m_enemyMgr, m_bulletMgr, m_isCameraHomePosition, MAP_SIZE, m_isTransitionStart);
	}

	// �t�B�[�o�[�̃g���K�[���擾
	if (!m_isFeverCameraEffect) {
		m_isFeverCameraEffect = m_player->GetIsFeverTrigger();
	}
	if (m_player->GetIsFeverEndTrigger()) {
		// �ʏ��Ԃɂ���B
		m_environmentMgr->ChangeStatus(EnvironmentMgr::DEFAULT);
	}

	// �t�B�[�o�[���̃J�������o����������B
	if (m_isFeverCameraEffect) {

		SlowMgr::Instance()->m_baseSlow = 0.0f;

		// ���݂̃^�C�}�[�̊��������߂�B
		float nowRate = static_cast<float>(m_feverNearCameraTimer) / static_cast<float>(FEVER_NEAR_CAMERA_TIMER);

		// ���݂̃V�F�C�N�ʁB
		float nowShake = nowRate * FEVER_SHAKE;
		Vec3<float> shake = Vec3<float>(KuroFunc::GetRand(-nowShake, nowShake), KuroFunc::GetRand(-nowShake, nowShake), KuroFunc::GetRand(-nowShake, nowShake));

		// �J�������v���C���[�ɋ߂Â���B	
		const float CAMERA_DISTANCE = 30.0f;
		m_baseEye = m_player->GetPos() + Vec3<float>(CAMERA_DISTANCE, CAMERA_DISTANCE, 0) + shake;
		m_nowEye += (m_baseEye - m_nowEye) / 3.0f;
		nowCam.SetPos(m_nowEye);

		// �J�����̒����_���ԁB
		m_baseTarget = m_player->GetPos();
		m_baseTarget.x += 3.0f;
		m_nowTarget += (m_baseTarget - m_nowTarget) / 3.0f;
		nowCam.SetTarget(m_nowTarget);


		// ��莞�Ԍo�߂�����t�B�[�o�[�̃J�������o���I����B
		++m_feverNearCameraTimer;
		if (FEVER_NEAR_CAMERA_TIMER < m_feverNearCameraTimer) {

			m_feverNearCameraTimer = 0;
			m_isFeverCameraEffect = false;
			SlowMgr::Instance()->m_baseSlow = 1.0f;

			// �t�B�[�o�[��Ԃɂ���B
			m_environmentMgr->ChangeStatus(EnvironmentMgr::FEVER);

		}

	}
	// �Q�[���̏�Ԃɉ����ăJ�����̈ʒu��ς���B
	else if (m_isTransitionStart) {

		// ������������Ă�����B
		if (m_isCompleteUpper) {

			// �J�������^�C�g���̃J�����̈ʒu�܂Ŏ����Ă����C�[�W���O�̃^�C�}�[���X�V�B
			m_returnTitlePosEasingTimer += 0.03f;
			if (1.0f <= m_returnTitlePosEasingTimer) m_returnTitlePosEasingTimer = 1.0f;
			float easingAmount = KuroMath::Ease(InOut, Exp, m_returnTitlePosEasingTimer, 0.0f, 1.0f);

			// �J�����̈ʒu���Z�b�g�B
			nowCam.SetTarget(m_baseEasingCameraTarget + (TITLE_TARGET_POS - m_baseEasingCameraTarget) * easingAmount);
			nowCam.SetPos(m_baseEasingCameraEye + (TITLE_EYE_POS - m_baseEasingCameraEye) * easingAmount);


			// ��Ԑ�Ƃ̋��������ȉ��ɂȂ�����J�ڂ�����B
			if (1.0f <= m_returnTitlePosEasingTimer) {

				++m_transitionDelayTimer;
				if (30 < m_transitionDelayTimer) {
					KuroEngine::Instance()->ChangeScene(0, m_sceneTransition.get());

				}
			}

		}
		else {

			Vec3<float> nowCameraTarget = nowCam.GetTarget();

			nowCameraTarget.y += (TRANSITION_CAMERA_Y - nowCameraTarget.y) / 5.0f;

			nowCam.SetTarget(nowCameraTarget);

			// ��Ԑ�܂ł̋��������ȉ��ɂȂ����玟��
			if (fabs(nowCameraTarget.y - TRANSITION_CAMERA_Y) <= 1.0f) {

				m_isInitPlayer = true;
				m_isCompleteUpper = true;

				m_baseEasingCameraEye = nowCam.GetPos();
				m_baseEasingCameraTarget = nowCam.GetTarget();

			}

		}


	}
	else if (m_isCameraHomePosition) {

		m_nowEye += (CAMERA_HOME_EYE_POSITION - m_nowEye) / 5.0f;
		m_nowTarget += (CAMERA_HOME_TARGET_POSITION - m_nowTarget) / 5.0f;
		m_gameCam->SetPos(m_nowEye);
		m_gameCam->SetTarget(m_nowTarget);

	}
	else {

		Vec3<float> playerVecX = -m_player->GetForwardVec();
		const float CAMERA_DISTANCE = 80.0f;

		// �J�������ԁB
		m_baseEye = m_player->GetPos() + Vec3<float>(CAMERA_DISTANCE, CAMERA_DISTANCE, 0);
		m_nowEye += (m_baseEye - m_nowEye) / 3.0f;

		m_baseTarget = m_player->GetPos();
		m_baseTarget.x += 3.0f;
		m_nowTarget += (m_baseTarget - m_nowTarget) / 3.0f;

		Vec3<float> eyeVec = Vec3<float>(m_baseEye - m_player->GetPos()).GetNormal();
		float xzAngle = atan2f(eyeVec.x, eyeVec.z);

		m_gameCam->SetPos(m_nowEye);
		m_gameCam->SetTarget(m_nowTarget);

	}

	// �`���[�g���A����Ԃ̎��A�G���^�[�L�[���������ƂŃQ�[�����[�h�̃J�����Ɉڍs����B
	if (UsersInput::Instance()->KeyInput(DIK_RETURN)) {
		m_isCameraHomePosition = true;
	}
	if (UsersInput::Instance()->KeyInput(DIK_SPACE)) {
		m_isCameraHomePosition = false;
	}

	m_environmentMgr->Update(m_player->GetPos());

	m_feverGauge->Update(m_player->GetIsFever(), m_player->GetPlayerFeverRate());


	SlowMgr::Instance()->Update();

	ShakeMgr::Instance()->Update();

	// �J�����𓮂����B
	m_gameCam->SetPos(m_gameCam->GetPos() + ShakeMgr::Instance()->m_shake);

}

#include "DrawFunc2D.h"
void TutorialScene::OnDraw()
{
	/*===== �`�揈�� =====*/

	//�o�b�N�o�b�t�@�擾
	auto backBuff = D3D12App::Instance()->GetBackBuffRenderTarget();

	//���݂̃J�����擾
	GameManager::Instance()->ChangeCamera(m_gameCamKey);
	auto& nowCam = GameManager::Instance()->GetNowCamera();

	//DrawFunc������
	DrawFunc_Append::FrameInit(
		backBuff,
		nowCam,
		m_environmentMgr->GetLigMgr()
	);

	KuroEngine::Instance()->Graphics().ClearRenderTarget(m_emissiveMap);

	/*--- �ʏ�`�� ---*/

	// �`���[�g���A���̕`��
	m_tutorial->Draw();

	// �}�b�v��`��
	StageFloor::Instance()->Draw();

	//���`��
	m_environmentMgr->Draw(*nowCam);

	//�v���C���[�`��
	m_player->Draw(*nowCam);

	//�G��`��
	m_enemyMgr->Draw(*nowCam, backBuff, m_emissiveMap, m_depthStencil);

	//�e��`��B
	m_bulletMgr->Draw();

	// �`���[�g���A���̖߂�̃A�C�R����`��
	m_tutorial->DrawReturnIcon();

	// �t�B�[�o�[�Q�[�W��`��B
	m_feverGauge->Draw();
	float radian = Angle::ConvertToRadian(90);
	float cosRadian = cosf(m_player->GetInputRadian() + radian);
	float sinRadian = sinf(m_player->GetInputRadian() + radian);
	Vec3<float>vel(cosRadian * 100.0f, 0.0f, sinRadian * -100.0f);
	//DrawFunc3D::DrawLine(*nowCam, m_player->GetPos(),
	//	m_player->GetPos() + vel
	//	, Color(255, 0, 255, 255), 1.0f);


	/*--- �G�~�b�V�u�}�b�v���� ---*/
		//���C�g�u���[���f�o�C�X���g���ĉ��Z����
	if (m_emissive)
	{
		m_ligBloomDev.Draw(m_emissiveMap, backBuff);
	}

	//	/* --- �f�o�b�O�`�� ---*/
	//#ifdef _DEBUG
	//	//�f�v�X�X�e���V���N���A
	//	KuroEngine::Instance()->Graphics().ClearDepthStencil(m_depthStencil);
	//	//�����_�[�^�[�Q�b�g�Z�b�g�i�o�b�N�o�b�t�@�ƃf�v�X�X�e���V���j
	//	KuroEngine::Instance()->Graphics().SetRenderTargets({ backBuff }, m_depthStencil);
	//
	//	//�f�o�b�O�`��t���O�m�F
	//	if (GameManager::Instance()->GetDebugDrawFlg())
	//	{
	//		//XYZ��
	//		static const float LEN = 100.0f;
	//		static const float THICKNESS = 0.5f;
	//		static Vec3<float>ORIGIN = { 0,0,0 };
	//		DrawFunc3D::DrawLine(*nowCam, ORIGIN, { LEN,0,0 }, Color(1.0f, 0.0f, 0.0f, 1.0f), THICKNESS);
	//		DrawFunc3D::DrawLine(*nowCam, ORIGIN, { 0,LEN,0 }, Color(0.0f, 1.0f, 0.0f, 1.0f), THICKNESS);
	//		DrawFunc3D::DrawLine(*nowCam, ORIGIN, { 0,0,LEN }, Color(0.0f, 0.0f, 1.0f, 1.0f), THICKNESS);
	//
	//		m_player->DrawDebugInfo(*nowCam);
	//
	//	}
	//#endif
}

void TutorialScene::OnImguiDebug()
{
	ImGui::Begin("Test");
	ImGui::Checkbox("Emissive", &m_emissive);
	bool isBrakeBoostMode = 0 < m_player->GetBrakeBoostTimer();
	ImGui::Checkbox("isBrakeBoostMode", &isBrakeBoostMode);
	ImGui::DragFloat("MapSize", &MAP_SIZE, 1.0f);
	ImGui::DragFloat("PlayerMaxSpeed", &m_player->MAX_SPEED, 0.1f);
	ImGui::DragFloat("PlayerMinSpeed", &m_player->MIN_SPEED, 0.1f);
	ImGui::DragFloat("PlayerDriftSpeed", &m_player->BRAKE_SPEED, 0.1f);
	ImGui::End();

	//�Q�[���}�l�[�W��imgui�f�o�b�O
	GameManager::Instance()->ImGuiDebug();
	ImGui::Begin("Player");
	ImGui::Text("POS_X:%f", m_player->GetPos().x);
	ImGui::Text("POS_Y:%f", m_player->GetPos().y);
	ImGui::Text("POS_Z:%f", m_player->GetPos().z);
	ImGui::End();
}

void TutorialScene::OnFinalize()
{
}