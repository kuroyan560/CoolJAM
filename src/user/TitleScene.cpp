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

TitleScene::TitleScene()
{

	//�o�b�N�o�b�t�@���擾
	auto backBuff = D3D12App::Instance()->GetBackBuffRenderTarget();

	//�f�v�X�X�e���V�������i�o�b�N�o�b�t�@�Ɠ����T�C�Y�j
	m_depthStencil = D3D12App::Instance()->GenerateDepthStencil(backBuff->GetGraphSize());

	m_gameCam = std::make_shared<Camera>(m_gameCamKey);
	GameManager::Instance()->RegisterCamera(m_gameCamKey, m_gameCam);
	GameManager::Instance()->ChangeCamera(m_gameCamKey);

	m_mapModel = Importer::Instance()->LoadModel("resource/user/map/", "mapModel.glb");
	m_mapModelTransform.SetScale(150.0f);

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

	// UI�p�̃e�N�X�`�������[�h
	m_selectUI[0] = D3D12App::Instance()->GenerateTextureBuffer("resource/user/UI/title_select_tutorial.png");
	m_selectUI[1] = D3D12App::Instance()->GenerateTextureBuffer("resource/user/UI/title_select_Gamel.png");
	m_selectUI[2] = D3D12App::Instance()->GenerateTextureBuffer("resource/user/UI/title_select_Exit.png");

	m_rotateUI[0] = OFF_SCREEN_ROTATE[0];
	m_rotateUI[1] = OFF_SCREEN_ROTATE[1];
	m_rotateUI[2] = OFF_SCREEN_ROTATE[2];

	m_revolverPos = OFF_SCREEN_REVOLVER_POS;

	m_isAppear = true;

	m_nowSelect = SELECT::TUTORIAL;

	m_addRotateUI = 0;

}

void TitleScene::OnInitialize()
{

	m_environmentMgr->Init();
	m_isTransition = false;
	m_transitionEasingTimer = 0;
	m_endEasingTransitionTimer = END_EASING_TIMER;

	m_rotateUI[0] = OFF_SCREEN_ROTATE[0];
	m_rotateUI[1] = OFF_SCREEN_ROTATE[1];
	m_rotateUI[2] = OFF_SCREEN_ROTATE[2];

	m_revolverPos = OFF_SCREEN_REVOLVER_POS;

	m_isAppear = true;

	m_nowSelect = SELECT::TUTORIAL;

	m_addRotateUI = 0;

}

void TitleScene::OnUpdate()
{

	//���݂̃J�����擾
	auto& nowCam = *GameManager::Instance()->GetNowCamera();

	//�Q�[���}�l�[�W���X�V
	GameManager::Instance()->Update();

	m_environmentMgr->Update();

	// �o������������B
	if (m_isAppear) {

		// �C�[�W���O�ʂ����߂�B
		float easingAmount = KuroMath::Ease(InOut, Exp, m_revolverEasingTimer, 0.0f, 1.0f);

		// �ʒu�����߂�B
		/*m_revolverPos = */

		// �^�C�}�[���X�V�B
		m_revolverEasingTimer += ADD_REVOLVER_EASING_TIMER;
		if (1.0f <= m_revolverEasingTimer) {

			m_revolverEasingTimer = 1.0f;
			m_isAppear = false;

		}

	}
	// �J�ڒ���������B
	else if (m_isTransition) {



	}
	else {

		// �I�𒆂̍X�V����
		UpdateSelect();

	}

	// �J�����̍X�V����
	UpdateCamera();


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
	DrawFunc_Append::DrawModel(m_mapModel, m_mapModelTransform);

	// �v���C���[��`��
	m_player->Draw(*nowCam, true);

	// UI�p��`��
	//for (auto& index : m_selectUI) {

	//	// �x�N�g�������߂�B
	//	Vec2<float> vec = Vec2<float>(cosf(m_rotateUI[static_cast<int>(&index - &m_selectUI[0])]), sinf(m_rotateUI[static_cast<int>(&index - &m_selectUI[0])]));

	//	// �`����W�����߂�B
	//	Vec2<float> centerPos = vec * 300.0f;
	//	centerPos.y += 720.0f / 2.0f;

	//	DrawFunc2D::DrawRotaGraph2D(centerPos, Vec2<float>(1.0f, 1.0f), m_rotateUI[static_cast<int>(&index - &m_selectUI[0])], index);

	//}


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


	if (UsersInput::Instance()->KeyInput(DIK_SPACE) || UsersInput::Instance()->KeyInput(DIK_RETURN)) {

		m_isTransition = true;
		//KuroEngine::Instance()->ChangeScene(1, m_sceneTransition.get());

		if (m_nowSelect == SELECT::EXIT) {

			exit(0);

		}

	}

	if (UsersInput::Instance()->KeyOnTrigger(DIK_UP)) {

		m_addRotateUI -= DirectX::XM_PIDIV2;

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

	}
	if (UsersInput::Instance()->KeyOnTrigger(DIK_DOWN)) {

		m_addRotateUI += DirectX::XM_PIDIV2;

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

	}

	float rotate = m_addRotateUI / 5.0f;
	m_addRotateUI -= m_addRotateUI / 5.0f;
	for (auto& index : m_rotateUI) {

		index += rotate;

		if (0 < rotate) {

			// �p�x���X�L�b�v�̈�ɓ����Ă�����B
			if (MIN_SKIP_ROTATE <= index && index < MAX_SKIP_ROTATE) {
				index = MAX_SKIP_ROTATE + (index - MIN_SKIP_ROTATE);
			}
			// �ō��l�𒴂��Ă�����B
			if (DirectX::XM_2PI <= index) {
				index = index - DirectX::XM_2PI;
			}

		}
		else if (rotate < 0) {

			// �p�x���X�L�b�v�̈�ɓ����Ă�����B
			if (index <= MAX_SKIP_ROTATE && MIN_SKIP_ROTATE < index) {
				index = MIN_SKIP_ROTATE - (MAX_SKIP_ROTATE - index);
			}
			// �Œ�l�𒴂��Ă�����B
			if (index <= 0) {
				index = DirectX::XM_2PI + index;
			}

		}

	}



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

				KuroEngine::Instance()->ChangeScene(1, m_sceneTransition.get());

			}

		}

		// �C�[�W���O�ʂ����߂�B
		float easingAmount = KuroMath::Ease(InOut, Cubic, m_transitionEasingTimer, 0.0f, 1.0f);

		// �����_�̍��������߂�B
		Vec3<float> nowTargetPos = nowCam.GetTarget();
		nowTargetPos = DEF_TARGET_POS + (END_TARGET_POS - DEF_TARGET_POS) * easingAmount;
		nowCam.SetTarget(nowTargetPos);

		// ���_�̍��������߂�B
		float nowEyeHeight = DEF_EYE_POS.y + (END_EYE_POS.y - DEF_EYE_POS.y) * easingAmount;

		// ���݂̊p�x�B
		float nowAngle = DEF_ANGLE + (END_ANGLE - DEF_ANGLE) * easingAmount;

		// ���݂̒����B
		float nowLength = DEF_LENGTH + (END_LENGTH - DEF_LENGTH) * easingAmount;

		// ���݂̎��_���W�����߂�B
		Vec3<float> nowEyePos = Vec3<float>(cosf(nowAngle) * nowLength, nowEyeHeight, sinf(nowAngle) * nowLength);
		nowCam.SetPos(nowEyePos);

	}
	else {


		// �C�[�W���O�ʂ����߂�B �J�ڒ��Ƃ̃Y���𖳂������߂�
		float easingAmount = 0;

		// �����_�̍��������߂�B
		Vec3<float> nowTargetPos = nowCam.GetTarget();
		nowTargetPos = DEF_TARGET_POS + (END_TARGET_POS - DEF_TARGET_POS) * easingAmount;
		nowCam.SetTarget(nowTargetPos);

		// ���_�̍��������߂�B
		float nowEyeHeight = DEF_EYE_POS.y + (END_EYE_POS.y - DEF_EYE_POS.y) * easingAmount;

		// ���݂̊p�x�B
		float nowAngle = DEF_ANGLE + CHANGE_ANGLE * easingAmount;

		// ���݂̒����B
		float nowLength = DEF_LENGTH + (END_LENGTH - DEF_LENGTH) * easingAmount;

		// ���݂̎��_���W�����߂�B
		Vec3<float> nowEyePos = Vec3<float>(cosf(nowAngle) * nowLength, nowEyeHeight, sinf(nowAngle) * nowLength);
		nowCam.SetPos(nowEyePos);

		m_endEasingTransitionTimer = END_EASING_TIMER;

	}


}