#include "GameScene.h"
#include"GameManager.h"
#include"DrawFunc3D.h"
#include"DrawFunc2D.h"

GameScene::GameScene()
{
	//�o�b�N�o�b�t�@���擾
	auto backBuff = D3D12App::Instance()->GetBackBuffRenderTarget();

	//���C�������_�[�^�[�Q�b�g
	m_mainTarget = D3D12App::Instance()->GenerateRenderTarget(DXGI_FORMAT_R32G32B32A32_FLOAT, Color(23, 14, 41, 255), backBuff->GetGraphSize(), L"MainTarget");

	//�f�v�X�X�e���V�������i�o�b�N�o�b�t�@�Ɠ����T�C�Y�j
	m_depthStencil = D3D12App::Instance()->GenerateDepthStencil(backBuff->GetGraphSize());

	//�G�~�b�V�u�}�b�v����
	m_emissiveMap = D3D12App::Instance()->GenerateRenderTarget(DXGI_FORMAT_R32G32B32A32_FLOAT, Color(0, 0, 0, 1), backBuff->GetGraphSize(), L"EmissiveMap");
}

void GameScene::OnInitialize()
{
	m_player.Init();
}

void GameScene::OnUpdate()
{
	GameManager::Instance()->Update();
	m_player.Update();
}


void GameScene::OnDraw()
{
	//�o�b�N�o�b�t�@�擾
	auto backBuff = D3D12App::Instance()->GetBackBuffRenderTarget();

	//���݂̃J�����擾
	auto& nowCam = *GameManager::Instance()->GetNowCamera();

	//���C�������_�[�^�[�Q�b�g�N���A
	KuroEngine::Instance()->Graphics().ClearRenderTarget(m_mainTarget);


/*--- �ʏ�`�� ---*/
	//�f�v�X�X�e���V���N���A
	KuroEngine::Instance()->Graphics().ClearDepthStencil(m_depthStencil);
	//�����_�[�^�[�Q�b�g�Z�b�g�i�o�b�N�o�b�t�@�ƃf�v�X�X�e���V���j
	KuroEngine::Instance()->Graphics().SetRenderTargets({ m_mainTarget }, m_depthStencil);

	//�v���C���[�`��
	m_player.Draw(nowCam);

/*--- �G�~�b�V�u�}�b�v�ɕ`�� ---*/
	//�f�v�X�X�e���V���N���A
	KuroEngine::Instance()->Graphics().ClearDepthStencil(m_depthStencil);
	//�G�~�b�V�u�}�b�v�N���A
	KuroEngine::Instance()->Graphics().ClearRenderTarget(m_emissiveMap);
	//�����_�[�^�[�Q�b�g�Z�b�g�i�G�~�b�V�u�}�b�v�ƃf�v�X�X�e���V���j
	KuroEngine::Instance()->Graphics().SetRenderTargets({ m_emissiveMap }, m_depthStencil);

	//�v���C���[�`��
	m_player.Draw(nowCam);

/*--- �G�~�b�V�u�}�b�v���� ---*/
	//���C�g�u���[���f�o�C�X���g���ĉ��Z����
	if (m_emissive)
	{
		m_ligBloomDev.Draw(m_emissiveMap, m_mainTarget);
	}

/*--- �o�b�N�o�b�t�@�ɉf�� ---*/
	KuroEngine::Instance()->Graphics().SetRenderTargets({ backBuff });
	DrawFunc2D::DrawGraph({ 0,0 }, m_mainTarget);

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
		static const float LEN = 30.0f;
		static const float THICKNESS = 0.02f;
		static Vec3<float>ORIGIN = { 0,0,0 };
		DrawFunc3D::DrawLine(nowCam, ORIGIN, { LEN,0,0 }, Color(1.0f, 0.0f, 0.0f, 1.0f), THICKNESS);
		DrawFunc3D::DrawLine(nowCam, ORIGIN, { 0,LEN,0 }, Color(0.0f, 1.0f, 0.0f, 1.0f), THICKNESS);
		DrawFunc3D::DrawLine(nowCam, ORIGIN, { 0,0,LEN }, Color(0.0f, 0.0f, 1.0f, 1.0f), THICKNESS);
	}
#endif
}

void GameScene::OnImguiDebug()
{
	return;
	ImGui::Begin("Test");
	ImGui::Checkbox("Emissive", &m_emissive);
	ImGui::End();

	//�Q�[���}�l�[�W��imgui�f�o�b�O
	GameManager::Instance()->ImGuiDebug();
}

void GameScene::OnFinalize()
{
}