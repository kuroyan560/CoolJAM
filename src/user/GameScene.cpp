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
#include"../engine/Common/Angle.h"
#include"KazCollisionHelper.h"
#include"../engine/DrawFuncBillBoard.h"

GameScene::GameScene()
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

	//�G�E�F�[�u�Ǘ��N���X
	m_enemyWaveMgr = std::make_unique<EnemyWaveMgr>();

	//�G�~�b�V�u�}�b�v����
	m_emissiveMap = D3D12App::Instance()->GenerateRenderTarget(DXGI_FORMAT_R32G32B32A32_FLOAT, Color(0, 0, 0, 1), backBuff->GetGraphSize(), L"EmissiveMap");

	//���}�l�[�W������
	m_environmentMgr = std::make_unique<EnvironmentMgr>();
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
	m_player->Init();
	m_grazeEmitter->Init(m_player->GetPosPtr(), m_player->GetInputRadianPtr());

}

void GameScene::OnUpdate()
{
	/*===== �X�V���� =====*/
	GameManager::Instance()->Update();

	//�X�N���[���T�C�Y���擾�B
	Vec2<float> windowSize = Vec2<float>(WinApp::Instance()->GetWinSize().x, WinApp::Instance()->GetWinSize().y);

	float distance = MAP_SIZE - m_player->GetPos().Length();
	m_grazeEmitter->Update(MAP_SIZE, distance <= 20.0f);


	//���݂̃J�����擾
	auto &nowCam = *GameManager::Instance()->GetNowCamera();

	nowCam.SetPos(m_player->GetPos() + Vec3<float>(30, 30, 0));
	nowCam.SetTarget(m_player->GetPos());

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
	m_grazeEmitter->Draw(nowCam);

	float radian = Angle::ConvertToRadian(90);
	float cosRadian = cosf(m_player->GetInputRadian() + radian);
	float sinRadian = sinf(m_player->GetInputRadian() + radian);
	Vec3<float>vel(cosRadian * 100.0f, 0.0f, sinRadian * -100.0f);
	DrawFunc3D::DrawLine(nowCam, m_player->GetPos(),
		m_player->GetPos() + vel
		, Color(255, 0, 255, 255), 1.0f);

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
	ImGui::Begin("Player");
	ImGui::Text("POS_X:%f", m_player->GetPos().x);
	ImGui::Text("POS_Y:%f", m_player->GetPos().y);
	ImGui::Text("POS_Z:%f", m_player->GetPos().z);
	ImGui::End();
}

void GameScene::OnFinalize()
{
}