#include "TitleScene.h"
#include "Player.h"
#include "Camera.h"
#include "GameManager.h"
#include "EnvironmentMgr.h"
#include "DrawFunc_Append.h"
#include "DrawFunc3D.h"
#include "Object.h"
#include "../engine/Importer.h"
#include "KuroMath.h"
#include "GameMode.h"
#include"AudioApp.h"
#include "TitleUI.h"
#include "EnemyMgr.h"
#include "BulletMgr.h"
#include "EnemyWaveMgr.h"
#include"StageFloor.h"

TitleScene::TitleScene()
{

	//�o�b�N�o�b�t�@���擾
	auto backBuff = D3D12App::Instance()->GetBackBuffRenderTarget();

	//�f�v�X�X�e���V�������i�o�b�N�o�b�t�@�Ɠ����T�C�Y�j
	m_depthStencil = D3D12App::Instance()->GenerateDepthStencil(backBuff->GetGraphSize());

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

	//�f�v�X�}�b�v����
	m_depthMap = D3D12App::Instance()->GenerateRenderTarget(DXGI_FORMAT_R32_FLOAT, Color(0, 0, 0, 0), backBuff->GetGraphSize(), L"DepthMap");

	//���}�l�[�W������
	m_environmentMgr = std::make_unique<EnvironmentMgr>();

	// �V�[���J��
	m_sceneTransition = std::make_unique<SceneTransition>();

	// �v���C���[�𐶐��B
	m_player = std::make_unique<Player>();
	m_player->Init();

	m_isTransition = false;
	m_transitionEasingTimer = 0;
	m_endEasingTransitionTimer = END_EASING_TIMER;

	// �^�C�g���摜�����[�h
	m_titleTexture = D3D12App::Instance()->GenerateTextureBuffer("resource/user/UI/title.png");
	m_titleAngle = 0;
	m_isTitle = true;

	// UI�p�̃e�N�X�`�������[�h
	m_selectUI[0] = D3D12App::Instance()->GenerateTextureBuffer("resource/user/UI/title_select_tutorial.png");
	m_selectUI[1] = D3D12App::Instance()->GenerateTextureBuffer("resource/user/UI/title_select_Gamel.png");
	m_selectUI[2] = D3D12App::Instance()->GenerateTextureBuffer("resource/user/UI/title_select_Exit.png");

	m_revolverPos = OFF_SCREEN_REVOLVER_POS;

	m_isAppear = false;

	m_nowSelect = SELECT::GAME;

	m_titleUI[0] = std::make_shared<TitleUI>("resource/user/UI/title_select_tutorial.png", TitleUI::STATUS::UP, TitleUI::UI_STATUS::TUTORIAL);
	m_titleUI[1] = std::make_shared<TitleUI>("resource/user/UI/title_select_Gamel.png", TitleUI::STATUS::MIDDLE, TitleUI::UI_STATUS::GAME);
	m_titleUI[2] = std::make_shared<TitleUI>("resource/user/UI/title_select_Exit.png", TitleUI::STATUS::DOWN, TitleUI::UI_STATUS::EXIT);

	m_selectSE = AudioApp::Instance()->LoadAudio("resource/user/sound/select.wav");
	m_enterSE = AudioApp::Instance()->LoadAudio("resource/user/sound/enter.wav");
}

void TitleScene::OnInitialize()
{

	m_environmentMgr->Init();
	m_isTransition = false;
	m_transitionEasingTimer = 0;
	m_endEasingTransitionTimer = END_EASING_TIMER;

	m_revolverPos = OFF_SCREEN_REVOLVER_POS;
	m_revolverEasingTimer = 0;

	m_isTitle = true;
	m_titleAngle = DirectX::XM_PI;
	m_isAppear = false;

	m_nowSelect = SELECT::GAME;

	for (auto& index : m_titleUI) {

		index->Init();

	}

}

void TitleScene::OnUpdate()
{

	//���݂̃J�����擾
	auto& nowCam = *GameManager::Instance()->GetNowCamera();

	//�Q�[���}�l�[�W���X�V
	GameManager::Instance()->Update();

	//�X�N���[���T�C�Y���擾�B
	Vec2<float> windowSize = Vec2<float>(WinApp::Instance()->GetWinSize().x, WinApp::Instance()->GetWinSize().y);
	m_player->Update(nowCam, m_bulletMgr, m_enemyMgr, windowSize, MAP_SIZE, 0, false);
	m_player->Init();

	m_environmentMgr->Update(m_player->GetPos());

	// �^�C�g����������B
	if (m_isTitle) {

		m_titleAngle -= m_titleAngle / 5.0f;

	}

	// �o������������B
	if (m_isAppear) {

		// �C�[�W���O�ʂ����߂�B
		float easingAmount = KuroMath::Ease(Out, Cubic, m_revolverEasingTimer, 0.0f, 1.0f);

		// �ʒu�����߂�B
		m_revolverPos = OFF_SCREEN_REVOLVER_POS + (REVOLVER_POS - OFF_SCREEN_REVOLVER_POS) * easingAmount;

		// �^�C�g������]������B
		m_titleAngle = DirectX::XM_2PI - DirectX::XM_PI * easingAmount;

		// �^�C�}�[���X�V�B
		m_revolverEasingTimer += ADD_REVOLVER_EASING_TIMER;
		if (1.0f <= m_revolverEasingTimer) {

			m_revolverEasingTimer = 1.0f;
			m_isAppear = false;

		}

	}
	// �J�ڒ���������B
	else if (m_isTransition) {

		// �C�[�W���O�ʂ����߂�B
		float easingAmount = KuroMath::Ease(In, Cubic, m_revolverEasingTimer, 0.0f, 1.0f);

		// �ʒu�����߂�B
		m_revolverPos = REVOLVER_POS + (OFF_SCREEN_REVOLVER_POS - REVOLVER_POS) * easingAmount;

		// �^�C�}�[���X�V�B
		m_revolverEasingTimer += ADD_REVOLVER_EASING_TIMER;
		if (1.0f <= m_revolverEasingTimer) {

			m_revolverEasingTimer = 1.0f;

		}

		// UI����]������B
		bool canTrans = true;
		for (auto& index : m_titleUI) {

			canTrans &= index->GetCanTrans();

		}
		if (canTrans) {

			for (auto& index : m_titleUI) {

				index->TransDown();

			}

		}

	}

	// �I�𒆂̍X�V����
	UpdateSelect();


	// �J�����̍X�V����
	UpdateCamera();



	for (auto& index : m_titleUI) {

		index->Update();

	}


}

#include "DrawFunc2D.h"
#include "DrawFunc3D.h"
void TitleScene::OnDraw()
{

	//�f�v�X�X�e���V���N���A
	KuroEngine::Instance()->Graphics().ClearDepthStencil(m_depthStencil);

	//�o�b�N�o�b�t�@�擾
	auto backBuff = D3D12App::Instance()->GetBackBuffRenderTarget();

	//���݂̃J�����擾
	auto& nowCam = GameManager::Instance()->GetNowCamera();

	//DrawFunc������
	DrawFunc_Append::FrameInit(
		backBuff,
		nowCam,
		m_environmentMgr->GetLigMgr()
	);

	//���`��
	m_environmentMgr->Draw(*nowCam);

	// �}�b�v��`��
	StageFloor::Instance()->Draw();

	// �v���C���[��`��
	m_player->Draw(*nowCam);

	// �^�C�g����`��B
	Vec2<float> vec = Vec2<float>(cosf(m_titleAngle), sinf(m_titleAngle));
	Vec2<float> centerPos = vec * 380.0f + Vec2<float>(0, 720.0f / 2.0f);
	DrawFunc2D::DrawRotaGraph2D(centerPos, Vec2<float>(1.2f, 1.2f), m_titleAngle, m_titleTexture);


	for (auto& index : m_titleUI) {

		index->Draw(m_revolverPos);

	}


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

void TitleScene::OnImguiDebug()
{
	ImGui::Begin("Test");
	ImGui::Checkbox("Emissive", &m_emissive);
	ImGui::End();

	//�Q�[���}�l�[�W��imgui�f�o�b�O
	GameManager::Instance()->ImGuiDebug();
}

void TitleScene::OnFinalize()
{
}

void TitleScene::UpdateSelect() {


	if (UsersInput::Instance()->MouseOnTrigger(LEFT)) {

		AudioApp::Instance()->PlayWaveDelay(m_enterSE);

		// �o��������Ȃ�������B
		if (!m_isTitle && !m_isAppear && !m_isTransition) {

			m_isTransition = true;

			m_revolverEasingTimer = 0;

			if (m_nowSelect == SELECT::GAME) {
				GameMode::Instance()->m_id = GameMode::ID::GAME;
			}
			if (m_nowSelect == SELECT::TUTORIAL) {
				GameMode::Instance()->m_id = GameMode::ID::TUTORIAL;
			}

			if (m_nowSelect == SELECT::EXIT) {

				KuroEngine::Instance()->GameEnd();

			}

		}
		// �^�C�g���`�撆��������B
		if (m_isTitle) {

			m_isAppear = true;
			m_isTitle = false;
		}

	}

	// �J�ډ\��
	bool canTrans = true;
	for (auto& index : m_titleUI) {

		canTrans &= index->GetCanTrans();

	}
	auto oldSelect = m_nowSelect;

	float mouseMove = UsersInput::Instance()->GetMouseMove().m_inputZ;
	if (mouseMove < -100.0f && !m_isAppear && !m_isTransition && !m_isTitle && canTrans) {

		switch (m_nowSelect)
		{
		case SELECT::EXIT:
			m_nowSelect = SELECT::TUTORIAL;
			break;
		case SELECT::GAME:
			m_nowSelect = SELECT::EXIT;
			break;
		case SELECT::TUTORIAL:
			m_nowSelect = SELECT::GAME;
			break;
		default:
			break;
		}

		for (auto& index : m_titleUI) {

			index->TransUp();

		}

	}

	if (100.0f < mouseMove && !m_isAppear && !m_isTransition && !m_isTitle && canTrans) {

		switch (m_nowSelect)
		{
		case SELECT::EXIT:
			m_nowSelect = SELECT::GAME;
			break;
		case SELECT::GAME:
			m_nowSelect = SELECT::TUTORIAL;
			break;
		case SELECT::TUTORIAL:
			m_nowSelect = SELECT::EXIT;
			break;
		default:
			break;
		}

		for (auto& index : m_titleUI) {

			index->TransDown();

		}

	}

	if (oldSelect != m_nowSelect)AudioApp::Instance()->PlayWaveDelay(m_selectSE);

}

void TitleScene::UpdateCamera() {

	//���݂̃J�����擾
	auto& nowCam = *GameManager::Instance()->GetNowCamera();

	// �J�ڒ���������B
	if (m_isTransition) {

		m_transitionEasingTimer += ADD_TRANSITION_EASING_TIMER;
		if (1.0f < m_transitionEasingTimer) {

			m_transitionEasingTimer = 1.0f;

			// �C�[�W���O��̑J�ڂ܂ł̃^�C�}�[���X�V����B
			--m_endEasingTransitionTimer;
			if (m_endEasingTransitionTimer <= 0) {

				// �J�ڐ�̃V�[�������߂�B
				int transitionSceneNum = 0;
				if (GameMode::Instance()->m_id == GameMode::ID::GAME) {
					transitionSceneNum = 2;
				}
				else if (GameMode::Instance()->m_id == GameMode::ID::TUTORIAL) {
					transitionSceneNum = 1;
				}

				KuroEngine::Instance()->ChangeScene(transitionSceneNum, m_sceneTransition.get());

			}

		}

		// ��Ԑ撍���_���W
		bool isGame = GameMode::Instance()->m_id == GameMode::ID::GAME;
		Vec3<float> endTarget = (isGame ? END_GAME_TARGET_POS : END_TUTORIAL_TARGET_POS);
		Vec3<float> endEye = (isGame ? END_GAME_EYE_POS : END_TUTORIAL_EYE_POS);
		float endLength = (isGame ? END_GAME_LENGTH : END_TUTORIAL_LENGTH);

		// �C�[�W���O�ʂ����߂�B
		float easingAmount = KuroMath::Ease(InOut, Cubic, m_transitionEasingTimer, 0.0f, 1.0f);

		// �����_�̍��������߂�B
		Vec3<float> nowTargetPos = nowCam.GetTarget();
		nowTargetPos = DEF_TARGET_POS + (endTarget - DEF_TARGET_POS) * easingAmount;
		nowCam.SetTarget(nowTargetPos);

		// ���_�̍��������߂�B
		float nowEyeHeight = DEF_EYE_POS.y + (endEye.y - DEF_EYE_POS.y) * easingAmount;

		// ���݂̊p�x�B
		float nowAngle = DEF_ANGLE + (END_ANGLE - DEF_ANGLE) * easingAmount;

		// ���݂̒����B
		float nowLength = DEF_LENGTH + (endLength - DEF_LENGTH) * easingAmount;

		// ���݂̎��_���W�����߂�B
		Vec3<float> nowEyePos = Vec3<float>(cosf(nowAngle) * nowLength, nowEyeHeight, sinf(nowAngle) * nowLength);
		nowCam.SetPos(nowEyePos);

	}
	else {


		// �C�[�W���O�ʂ����߂�B �J�ڒ��Ƃ̃Y���𖳂������߂�
		float easingAmount = 0;

		// �����_�̍��������߂�B
		Vec3<float> nowTargetPos = nowCam.GetTarget();
		nowTargetPos = DEF_TARGET_POS + (END_GAME_TARGET_POS - DEF_TARGET_POS) * easingAmount;
		nowCam.SetTarget(nowTargetPos);

		// ���_�̍��������߂�B
		float nowEyeHeight = DEF_EYE_POS.y + (END_GAME_EYE_POS.y - DEF_EYE_POS.y) * easingAmount;

		// ���݂̊p�x�B
		float nowAngle = DEF_ANGLE + CHANGE_ANGLE * easingAmount;

		// ���݂̒����B
		float nowLength = DEF_LENGTH + (END_GAME_LENGTH - DEF_LENGTH) * easingAmount;

		// ���݂̎��_���W�����߂�B
		Vec3<float> nowEyePos = Vec3<float>(cosf(nowAngle) * nowLength, nowEyeHeight, sinf(nowAngle) * nowLength);
		nowCam.SetPos(nowEyePos);

		m_endEasingTransitionTimer = END_EASING_TIMER;

	}


}