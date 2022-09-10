#include"GameScene.h"
#include"GameManager.h"
#include"DrawFunc3D.h"
#include"Player.h"
#include"Importer.h"
#include"DrawFunc2D.h"
#include"EnemyMgr.h"
#include"Enemy.h"
#include"BulletMgr.h"
#include"WinApp.h"
#include"EnvironmentMgr.h"
#include"EnemyWaveMgr.h"

GameScene::GameScene()
{
	//�o�b�N�o�b�t�@���擾
	auto backBuff = D3D12App::Instance()->GetBackBuffRenderTarget();

	//�f�v�X�X�e���V�������i�o�b�N�o�b�t�@�Ɠ����T�C�Y�j
	m_depthStencil = D3D12App::Instance()->GenerateDepthStencil(backBuff->GetGraphSize());

	m_player = std::make_unique<Player>();
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

	//�G�E�F�[�u�Ǘ��N���X
	m_enemyWaveMgr = std::make_unique<EnemyWaveMgr>(MAP_SIZE);

	//�G�~�b�V�u�}�b�v����
	m_emissiveMap = D3D12App::Instance()->GenerateRenderTarget(DXGI_FORMAT_R32G32B32A32_FLOAT, Color(0, 0, 0, 1), backBuff->GetGraphSize(), L"EmissiveMap");

	//���}�l�[�W������
	m_environmentMgr = std::make_unique<EnvironmentMgr>();

	m_gameTimer = std::make_unique<GameTimer>();
}

void GameScene::OnInitialize()
{

	/*===== ���������� =====*/
	m_player->Init();
	m_enemyMgr->Init();
	m_bulletMgr->Init();
	m_enemyWaveMgr->Init();

	m_baseEye = Vec3<float>(m_player->GetPos() + Vec3<float>(30.0f, 30.0f, 0.0f));
	m_nowEye = m_baseEye;
	m_baseTarget = m_player->GetPos();
	m_nowTarget = m_baseTarget;

	m_environmentMgr->Init();

	m_gameTimer->Init(120);
	m_gameTimer->Start();

}

void GameScene::OnUpdate()
{

	/*===== �X�V���� =====*/

	GameManager::Instance()->Update();

	//�X�N���[���T�C�Y���擾�B
	Vec2<float> windowSize = Vec2<float>(WinApp::Instance()->GetWinSize().x, WinApp::Instance()->GetWinSize().y);

	//���݂̃J�����擾
	auto& nowCam = *GameManager::Instance()->GetNowCamera();

	// �v���C���[�X�V����
	m_player->Update(nowCam, m_bulletMgr, m_enemyMgr, windowSize, MAP_SIZE, EDGE_SCOPE);

	// �G�E�F�[�u�Ǘ��N���X���X�V�B
	m_enemyWaveMgr->Update(m_enemyMgr, m_player->GetPos(), MAP_SIZE);

	// �G�X�V����
	m_enemyMgr->Update(m_bulletMgr, m_player->GetPos(), MAP_SIZE);

	// �e���X�V�B
	m_bulletMgr->Update(MAP_SIZE);

	Vec3<float> playerVecX = -m_player->GetForwardVec();
	const float CAMERA_DISTANCE = 60.0f;

	// �J�������ԁB
	m_baseEye = m_player->GetPos() + Vec3<float>(CAMERA_DISTANCE, CAMERA_DISTANCE, 0);
	m_nowEye = m_baseEye;

	m_baseTarget = m_player->GetPos();
	m_baseTarget.x += 3.0f;
	m_nowTarget = m_baseTarget;

	m_gameCam->SetPos(m_nowEye);
	m_gameCam->SetTarget(m_nowTarget);

	m_environmentMgr->Update();

	m_gameTimer->Update();
}

void GameScene::OnDraw()
{
	/*===== �`�揈�� =====*/

	//�f�v�X�X�e���V���N���A
	KuroEngine::Instance()->Graphics().ClearDepthStencil(m_depthStencil);

	//�o�b�N�o�b�t�@�擾
	auto backBuff = D3D12App::Instance()->GetBackBuffRenderTarget();

	//���݂̃J�����擾
	auto& nowCam = *GameManager::Instance()->GetNowCamera();

	/*--- �ʏ�`�� ---*/
		//�f�v�X�X�e���V���N���A
	KuroEngine::Instance()->Graphics().ClearDepthStencil(m_depthStencil);
	//�����_�[�^�[�Q�b�g�Z�b�g�i�o�b�N�o�b�t�@�ƃf�v�X�X�e���V���j
	KuroEngine::Instance()->Graphics().SetRenderTargets({ backBuff }, m_depthStencil);

	//���`��
	m_environmentMgr->Draw(nowCam);

	//�v���C���[�`��
	m_player->Draw(nowCam);

	//�G��`��
	m_enemyMgr->Draw(nowCam);

	//�e��`��B
	m_bulletMgr->Draw(nowCam);

	// �}�b�v��`��
	m_mapModel->m_transform.SetScale(MAP_SIZE);
	DrawFunc3D::DrawNonShadingModel(m_mapModel, nowCam);

	/*--- �G�~�b�V�u�}�b�v�ɕ`�� ---*/
		//�f�v�X�X�e���V���N���A
	KuroEngine::Instance()->Graphics().ClearDepthStencil(m_depthStencil);
	//�G�~�b�V�u�}�b�v�N���A
	KuroEngine::Instance()->Graphics().ClearRenderTarget(m_emissiveMap);
	//�����_�[�^�[�Q�b�g�Z�b�g�i�G�~�b�V�u�}�b�v�ƃf�v�X�X�e���V���j
	KuroEngine::Instance()->Graphics().SetRenderTargets({ m_emissiveMap }, m_depthStencil);

	//�v���C���[�`��
	m_player->Draw(nowCam);


	m_gameTimer->Draw();

	/*--- �G�~�b�V�u�}�b�v���� ---*/
		//���C�g�u���[���f�o�C�X���g���ĉ��Z����
	if (m_emissive)
	{
		m_ligBloomDev.Draw(m_emissiveMap, backBuff);
	}


	/* --- �f�o�b�O�`�� ---*/
#ifdef _DEBUG
	//�f�v�X�X�e���V���N���A
	KuroEngine::Instance()->Graphics().ClearDepthStencil(m_depthStencil);
	//�����_�[�^�[�Q�b�g�Z�b�g�i�o�b�N�o�b�t�@�ƃf�v�X�X�e���V���j
	KuroEngine::Instance()->Graphics().SetRenderTargets({ backBuff }, m_depthStencil);

	//�f�o�b�O�`��t���O�m�F
	if (GameManager::Instance()->GetDebugDrawFlg())
	{
		//XYZ��
		static const float LEN = 100.0f;
		static const float THICKNESS = 0.5f;
		static Vec3<float>ORIGIN = { 0,0,0 };
		DrawFunc3D::DrawLine(nowCam, ORIGIN, { LEN,0,0 }, Color(1.0f, 0.0f, 0.0f, 1.0f), THICKNESS);
		DrawFunc3D::DrawLine(nowCam, ORIGIN, { 0,LEN,0 }, Color(0.0f, 1.0f, 0.0f, 1.0f), THICKNESS);
		DrawFunc3D::DrawLine(nowCam, ORIGIN, { 0,0,LEN }, Color(0.0f, 0.0f, 1.0f, 1.0f), THICKNESS);

		m_player->DrawDebugInfo(nowCam);

	}
#endif
}

void GameScene::OnImguiDebug()
{
	ImGui::Begin("Test");
	ImGui::Checkbox("Emissive", &m_emissive);
	bool isBrakeBoostMode = 0 < m_player->GetBrakeBoostTimer();
	ImGui::Checkbox("isBrakeBoostMode", &isBrakeBoostMode);
	ImGui::DragFloat("MapSize", &MAP_SIZE, 1.0f);
	ImGui::DragFloat("PlayerMaxSpeed", &m_player->MAX_SPEED, 0.1f);
	ImGui::DragFloat("PlayerMinSpeed", &m_player->MIN_SPEED, 0.1f);
	ImGui::DragFloat("PlayerDriftSpeed", &m_player->BRAKE_SPEED, 0.1f);
	ImGui::Checkbox("ShootEnemy", &Enemy::m_debugIsShotEnemy);
	ImGui::End();

	//�Q�[���}�l�[�W��imgui�f�o�b�O
	GameManager::Instance()->ImGuiDebug();
}

void GameScene::OnFinalize()
{
}