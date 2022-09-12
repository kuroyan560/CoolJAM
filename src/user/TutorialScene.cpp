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
#include"TutorialText.h"

TutorialScene::TutorialScene()
{
	//�o�b�N�o�b�t�@���擾
	auto backBuff = D3D12App::Instance()->GetBackBuffRenderTarget();

	/*===== �R���X�g���N�^ =====*/

	//�f�v�X�X�e���V�������i�o�b�N�o�b�t�@�Ɠ����T�C�Y�j
	m_depthStencil = D3D12App::Instance()->GenerateDepthStencil(backBuff->GetGraphSize());

	m_player = std::make_unique<Player>();
	m_grazeEmitter = std::make_unique<GrazeEmitter>();
	m_player->Init();

	m_mapModel = std::make_shared<ModelObject>("resource/user/map/", "mapModel.glb");
	m_mapModel->m_transform.SetScale(100.0f);

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

	m_tutorialText = std::make_shared<TutorialText>();
	m_damageWallTimer = 0;
	m_enemyTutorialAppearTimer = 0;
	m_enemyTutorialExitTimer = 0;
	m_feverTutorialAppearTimer = 0;
	m_feverTutorialSpawnDelay = 0;
	m_isFever = false;

}

void TutorialScene::OnInitialize()
{

	/*===== ���������� =====*/
	m_player->Init();
	m_enemyMgr->Init();
	m_bulletMgr->Init();
	m_enemyWaveMgr->Init();
	m_feverGauge->Init();

	m_environmentMgr->Init();
	m_player->Init();
	m_grazeEmitter->Init(m_player->GetPosPtr(), m_player->GetInputRadianPtr());

	m_isCameraHomePosition = true;

	m_tutorialText->Init();
	m_damageWallTimer = 0;
	m_enemyTutorialAppearTimer = 0;
	m_enemyTutorialExitTimer = 0;
	m_feverTutorialAppearTimer = 0;
	m_feverTutorialSpawnDelay = 0;
	m_isFever = false;

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

}

void TutorialScene::OnUpdate()
{
	/*===== �X�V���� =====*/

	//���݂̃J�����擾
	auto& nowCam = *GameManager::Instance()->GetNowCamera();

	//�X�N���[���T�C�Y���擾�B
	Vec2<float> windowSize = Vec2<float>(WinApp::Instance()->GetWinSize().x, WinApp::Instance()->GetWinSize().y);

	m_grazeEmitter->Update(MAP_SIZE);

	//�Q�[���}�l�[�W���X�V
	GameManager::Instance()->Update();

	// �v���C���[�X�V����
	m_player->Update(nowCam, m_bulletMgr, m_enemyMgr, windowSize, MAP_SIZE, EDGE_SCOPE);

	// �G�X�V����
	m_enemyMgr->Update(m_bulletMgr, m_player->GetPos(), MAP_SIZE);

	// �e���X�V�B
	m_bulletMgr->Update(MAP_SIZE);


	switch (m_tutorialText->GetNowStatus())
	{
	case TutorialText::TUTORIAL_STATUS::MOUSE:

	{

		/*-- �}�E�X�̃`���[�g���A���������� --*/

		// ���ʈړ������玟�̃`���[�g���A���ցB
		const float MOVE_AMOUNT = 200.0f;
		if (MOVE_AMOUNT <= m_player->GetMovedLength()) {

			m_tutorialText->Next();
			m_player->ResetDashCount();

		}

	}

	break;
	case TutorialText::TUTORIAL_STATUS::DASH:

	{

		/*-- �_�b�V���̃`���[�g���A���������� --*/

		// �O��_�b�V�������玟�̃`���[�g���A���ցB
		const int DASH_COUNT = 3;
		if (DASH_COUNT <= m_player->GetDashCount()) {

			m_tutorialText->Next();

		}

	}

	break;
	case TutorialText::TUTORIAL_STATUS::DAMAGE_WALL:

	{

		/*-- �_���[�W�ǂ̃`���[�g���A���������� --*/

		++m_damageWallTimer;
		if (DAMAGE_WALL_TUTORIAL_TIMER <= m_damageWallTimer) {

			m_tutorialText->Next();

		}

	}

	break;
	case TutorialText::TUTORIAL_STATUS::ENEMY:

		/*-- �G�̃`���[�g���A���������� --*/

		++m_enemyTutorialAppearTimer;
		// �^�C�}�[�����傤�ǂɂȂ�����J�������񂹂ēG�𐶐�����B
		if (m_enemyTutorialAppearTimer == ENEMY_TUTORIAL_APPEAR_TIMER) {

			// �J�������񂹂āA�G�𐶐�����B
			m_isCameraHomePosition = false;
			m_enemyMgr->Generate(m_player->GetPos(), Vec3<float>(), Vec3<float>(), static_cast<int>(ENEMY_INFO::ID::STOPPING), MAP_SIZE);

		}
		// �^�C�}�[������l�𒴂��Ă��āA�G���S�Ď���ł�����B
		if (ENEMY_TUTORIAL_APPEAR_TIMER <= m_enemyTutorialAppearTimer) {

			// ���ցB
			if (m_enemyMgr->GetAllEnemyDead()) {

				// �G��|���Ă��玟�̃`���[�g���A���ɑJ�ڂ���܂ł̃^�C�}�[���X�V�B
				++m_enemyTutorialExitTimer;
				if (ENEMY_TUTORIAL_EXIT_TIMER <= m_enemyTutorialExitTimer) {

					m_tutorialText->Next();

				}

			}

		}

		break;
	case TutorialText::TUTORIAL_STATUS::FEVER:

		/*-- �t�B�[�o�[�̃`���[�g���A���������� --*/

		++m_feverTutorialAppearTimer;
		// �t�B�[�o�[�̃`���[�g���A���̃e�L�X�g��\�����鎞�ԁB
		if (m_feverTutorialAppearTimer < FEVER_TUTORIAL_APPEAR_TIMER) {

			// �J�����������B
			m_isCameraHomePosition = true;

		}
		// �^�C�}�[�����x�̂Ƃ��A�J�������񂹂ăt�B�[�o�[�G�𐶐�����B
		if (m_feverTutorialAppearTimer == FEVER_TUTORIAL_APPEAR_TIMER) {

			// �J�������񂹂�B
			m_isCameraHomePosition = false;

			// �G���L���𐶐�����B
			m_enemyMgr->Generate(m_player->GetPos(), Vec3<float>(), Vec3<float>(), static_cast<int>(ENEMY_INFO::ID::ELEC_MUSHI), MAP_SIZE);

		}

		// �t�B�[�o�[��ԂɂȂ�����G���ʂɐ�������B
		if (m_player->GetIsFever()) {

			m_isFever = true;

			++m_feverTutorialSpawnDelay;
			if (FEVER_TUTORIAL_SPAWN_DELAY <= m_feverTutorialSpawnDelay) {

				m_feverTutorialSpawnDelay = 0;

				// �G�𐶐�����B
				m_enemyMgr->Generate(m_player->GetPos(), Vec3<float>(100.0f, 0.0f, 0.0f), Vec3<float>(0, 0, -1), static_cast<int>(ENEMY_INFO::ID::TORUS_MOVE), MAP_SIZE);
				m_enemyMgr->Generate(m_player->GetPos(), Vec3<float>(-100.0f, 0.0f, 0.0f), Vec3<float>(0, 0, 1), static_cast<int>(ENEMY_INFO::ID::TORUS_MOVE), MAP_SIZE);
				m_enemyMgr->Generate(m_player->GetPos(), Vec3<float>(0.0f, 0.0f, 100.0f), Vec3<float>(1, 0, 0), static_cast<int>(ENEMY_INFO::ID::TORUS_MOVE), MAP_SIZE);
				m_enemyMgr->Generate(m_player->GetPos(), Vec3<float>(0.0f, 0.0f, -100.0f), Vec3<float>(-1, 0, 0), static_cast<int>(ENEMY_INFO::ID::TORUS_MOVE), MAP_SIZE);

				m_enemyMgr->Generate(m_player->GetPos(), Vec3<float>(0.0f, 0.0f, -50.0f), Vec3<float>(), static_cast<int>(ENEMY_INFO::ID::TRACKING), MAP_SIZE);
				m_enemyMgr->Generate(m_player->GetPos(), Vec3<float>(0.0f, 0.0f, 50.0f), Vec3<float>(), static_cast<int>(ENEMY_INFO::ID::TRACKING), MAP_SIZE);

			}

		}

		// �t�B�[�o�[�ɂȂ������Ƃ������āA���݃t�B�[�o�[����Ȃ�������G����C�ɏ����B
		if (m_isFever && !m_player->GetIsFever()) {

			m_enemyMgr->AllKill(m_bulletMgr);

		}


		break;
	default:
		break;
	}


	// �GWave�N���X�̍X�V�����B
	//m_enemyWaveMgr->Update(m_enemyMgr, m_player->GetPos(), MAP_SIZE);

	// �Q�[���̏�Ԃɉ����ăJ�����̈ʒu��ς���B
	if (m_isCameraHomePosition) {

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

	/*--- �ʏ�`�� ---*/

	// �}�b�v��`��
	m_mapModel->m_transform.SetScale(MAP_SIZE);
	DrawFunc_Append::DrawModel(m_mapModel, RenderTargetSwitch(), false, false);

	//���`��
	m_environmentMgr->Draw(*nowCam);

	//�v���C���[�`��
	m_player->Draw(*nowCam);

	//�G��`��
	m_enemyMgr->Draw(*nowCam, backBuff, m_emissiveMap, m_depthStencil);

	//�e��`��B
	m_bulletMgr->Draw();

	// �t�B�[�o�[�Q�[�W��`��B
	m_feverGauge->Draw();

	m_grazeEmitter->Draw(*nowCam);

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