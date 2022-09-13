#include"GameScene.h"
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

GameScene::GameScene()
{
	//ƒoƒbƒNƒoƒbƒtƒ@î•ñæ“¾
	auto backBuff = D3D12App::Instance()->GetBackBuffRenderTarget();

	/*===== ƒRƒ“ƒXƒgƒ‰ƒNƒ^ =====*/

	//ƒfƒvƒXƒXƒeƒ“ƒVƒ‹¶¬iƒoƒbƒNƒoƒbƒtƒ@‚Æ“¯‚¶ƒTƒCƒYj
	m_depthStencil = D3D12App::Instance()->GenerateDepthStencil(backBuff->GetGraphSize());

	m_player = std::make_unique<Player>();
	m_grazeEmitter = std::make_unique<GrazeEmitter>();
	m_player->Init();

	m_mapModel = std::make_shared<ModelObject>("resource/user/map/", "mapModel.glb");
	m_mapModel->m_transform.SetScale(100.0f);

	m_gameCam = std::make_shared<Camera>(m_gameCamKey);
	GameManager::Instance()->RegisterCamera(m_gameCamKey, m_gameCam);
	GameManager::Instance()->ChangeCamera(m_gameCamKey);

	//“G‚ğ¶¬B
	m_enemyMgr = std::make_shared<EnemyMgr>();
	m_enemyMgr->Init();

	//’eƒNƒ‰ƒX‚ğ¶¬B
	m_bulletMgr = std::make_shared<BulletMgr>();

	// ƒtƒB[ƒo[ƒQ[ƒW
	m_feverGauge = std::make_unique<FeverGauge>();

	//“GƒEƒF[ƒuŠÇ—ƒNƒ‰ƒX
	m_enemyWaveMgr = std::make_unique<EnemyWaveMgr>(MAP_SIZE);

	//ƒGƒ~ƒbƒVƒuƒ}ƒbƒv¶¬
	m_emissiveMap = D3D12App::Instance()->GenerateRenderTarget(DXGI_FORMAT_R32G32B32A32_FLOAT, Color(0, 0, 0, 1), backBuff->GetGraphSize(), L"EmissiveMap");

	//ƒfƒvƒXƒ}ƒbƒv¶¬
	m_depthMap = D3D12App::Instance()->GenerateRenderTarget(DXGI_FORMAT_R32_FLOAT, Color(0, 0, 0, 0), backBuff->GetGraphSize(), L"DepthMap");

	//ŠÂ‹«ƒ}ƒl[ƒWƒƒ¶¬
	m_environmentMgr = std::make_unique<EnvironmentMgr>();

	const auto backBuffFormat = backBuff->GetDesc().Format;
	DrawFunc_Append::RegisterRenderTargets(backBuffFormat, m_emissiveMap, m_depthMap, m_depthStencil);
	DrawFunc3D::GenerateDrawLinePipeline(backBuffFormat);
	DrawFunc3D::GenerateDrawLinePipeline(backBuffFormat, AlphaBlendMode_Add);
	
	//BGM“Ç‚İ‚İ
	m_bgm = AudioApp::Instance()->LoadAudio("resource/user/sound/bgm.wav", 0.1f);
}

void GameScene::OnInitialize()
{

	/*===== ‰Šú‰»ˆ— =====*/
	m_player->Init();
	m_enemyMgr->Init();
	m_bulletMgr->Init();
	m_enemyWaveMgr->Init();
	m_feverGauge->Init();

	m_environmentMgr->Init();
	m_player->Init();
	m_grazeEmitter->Init(m_player->GetPosPtr(), m_player->GetInputRadianPtr());

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

	AudioApp::Instance()->PlayWave(m_bgm, true);
}

void GameScene::OnUpdate()
{
	/*===== XVˆ— =====*/
	//Œ»İ‚ÌƒJƒƒ‰æ“¾
	auto &nowCam = *GameManager::Instance()->GetNowCamera();

	//ƒXƒNƒŠ[ƒ“ƒTƒCƒY‚ğæ“¾B
	Vec2<float> windowSize = Vec2<float>(WinApp::Instance()->GetWinSize().x, WinApp::Instance()->GetWinSize().y);

	m_grazeEmitter->Update(MAP_SIZE);

	//ƒQ[ƒ€ƒ}ƒl[ƒWƒƒXV
	GameManager::Instance()->Update();

	// ƒvƒŒƒCƒ„[XVˆ—
	m_player->Update(nowCam, m_bulletMgr, m_enemyMgr, windowSize, MAP_SIZE, EDGE_SCOPE);

	// “GXVˆ—
	m_enemyMgr->Update(m_bulletMgr, m_player->GetPos(), MAP_SIZE);

	// ’e‚ğXVB
	m_bulletMgr->Update(MAP_SIZE);

	// “GWaveƒNƒ‰ƒX‚ÌXVˆ—B
	m_enemyWaveMgr->Update(m_enemyMgr, m_player->GetPos(), MAP_SIZE);

	// ƒQ[ƒ€‚Ìó‘Ô‚É‰‚¶‚ÄƒJƒƒ‰‚ÌˆÊ’u‚ğ•Ï‚¦‚éB
	if (GameMode::Instance()->m_id == GameMode::ID::GAME) {

		Vec3<float> playerVecX = -m_player->GetForwardVec();
		const float CAMERA_DISTANCE = 80.0f;

		// ƒJƒƒ‰‚ğ•âŠÔB
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
	else {
		m_nowEye += (CAMERA_HOME_EYE_POSITION - m_nowEye) / 5.0f;
		m_nowTarget += (CAMERA_HOME_TARGET_POSITION - m_nowTarget) / 5.0f;
		m_gameCam->SetPos(m_nowEye);
		m_gameCam->SetTarget(m_nowTarget);

	}

	// ƒ`ƒ…[ƒgƒŠƒAƒ‹ó‘Ô‚ÌAƒGƒ“ƒ^[ƒL[‚ğ‰Ÿ‚·‚±‚Æ‚ÅƒQ[ƒ€ƒ‚[ƒh‚ÌƒJƒƒ‰‚ÉˆÚs‚·‚éB
	if (GameMode::Instance()->m_id == GameMode::ID::TUTORIAL && UsersInput::Instance()->KeyInput(DIK_RETURN)) {
		GameMode::Instance()->m_id = GameMode::ID::GAME;
	}
	if (GameMode::Instance()->m_id == GameMode::ID::GAME && UsersInput::Instance()->KeyInput(DIK_SPACE)) {
		GameMode::Instance()->m_id = GameMode::ID::TUTORIAL;
	}

	m_environmentMgr->Update(m_player->GetPos());


	m_feverGauge->Update(m_player->GetIsFever(), m_player->GetPlayerFeverRate());


}

#include "DrawFunc2D.h"
void GameScene::OnDraw()
{
	/*===== •`‰æˆ— =====*/

	//ƒoƒbƒNƒoƒbƒtƒ@æ“¾
	auto backBuff = D3D12App::Instance()->GetBackBuffRenderTarget();

	//Œ»İ‚ÌƒJƒƒ‰æ“¾
	auto &nowCam = GameManager::Instance()->GetNowCamera();

	//DrawFunc‰Šú‰»
	DrawFunc_Append::FrameInit(
		backBuff,
		nowCam,
		m_environmentMgr->GetLigMgr()
	);

	/*--- ’Êí•`‰æ ---*/

	//ŠÂ‹«•`‰æ
	m_environmentMgr->Draw(*nowCam);

	// ƒ}ƒbƒv‚ğ•`‰æ
	m_mapModel->m_transform.SetScale(MAP_SIZE);
	DrawFunc_Append::DrawModel(m_mapModel, RenderTargetSwitch(), false, true, AlphaBlendMode_Trans);

	//ƒvƒŒƒCƒ„[•`‰æ
	m_player->Draw(*nowCam);

	//“G‚ğ•`‰æ
	m_enemyMgr->Draw(*nowCam, backBuff, m_emissiveMap, m_depthStencil);

	//’e‚ğ•`‰æB
	m_bulletMgr->Draw();

	// ƒtƒB[ƒo[ƒQ[ƒW‚ğ•`‰æB
	m_feverGauge->Draw();

	m_grazeEmitter->Draw(*nowCam);

	float radian = Angle::ConvertToRadian(90);
	float cosRadian = cosf(m_player->GetInputRadian() + radian);
	float sinRadian = sinf(m_player->GetInputRadian() + radian);
	Vec3<float>vel(cosRadian * 100.0f, 0.0f, sinRadian * -100.0f);
	//DrawFunc3D::DrawLine(*nowCam, m_player->GetPos(),
	//	m_player->GetPos() + vel
	//	, Color(255, 0, 255, 255), 1.0f);


	/*--- ƒGƒ~ƒbƒVƒuƒ}ƒbƒv‡¬ ---*/
		//ƒ‰ƒCƒgƒuƒ‹[ƒ€ƒfƒoƒCƒX‚ğg‚Á‚Ä‰ÁZ‡¬
	if (m_emissive)
	{
		m_ligBloomDev.Draw(m_emissiveMap, backBuff);
	}

	//	/* --- ƒfƒoƒbƒO•`‰æ ---*/
	//#ifdef _DEBUG
	//	//ƒfƒvƒXƒXƒeƒ“ƒVƒ‹ƒNƒŠƒA
	//	KuroEngine::Instance()->Graphics().ClearDepthStencil(m_depthStencil);
	//	//ƒŒƒ“ƒ_[ƒ^[ƒQƒbƒgƒZƒbƒgiƒoƒbƒNƒoƒbƒtƒ@‚ÆƒfƒvƒXƒXƒeƒ“ƒVƒ‹j
	//	KuroEngine::Instance()->Graphics().SetRenderTargets({ backBuff }, m_depthStencil);
	//
	//	//ƒfƒoƒbƒO•`‰æƒtƒ‰ƒOŠm”F
	//	if (GameManager::Instance()->GetDebugDrawFlg())
	//	{
	//		//XYZ²
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
	ImGui::End();

	//ƒQ[ƒ€ƒ}ƒl[ƒWƒƒimguiƒfƒoƒbƒO
	GameManager::Instance()->ImGuiDebug();
	ImGui::Begin("Player");
	ImGui::Text("POS_X:%f", m_player->GetPos().x);
	ImGui::Text("POS_Y:%f", m_player->GetPos().y);
	ImGui::Text("POS_Z:%f", m_player->GetPos().z);
	ImGui::End();

	//ï¿½Eï¿½Fï¿½[ï¿½uï¿½ÒW
	EnemyWaveEditor::Instance()->EditWithImgui(*m_enemyWaveMgr, m_enemyMgr);
}

void GameScene::OnFinalize()
{
}