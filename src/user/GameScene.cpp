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

GameScene::GameScene()
{
	//バックバッファ情報取得
	auto backBuff = D3D12App::Instance()->GetBackBuffRenderTarget();

	//デプスステンシル生成（バックバッファと同じサイズ）
	m_depthStencil = D3D12App::Instance()->GenerateDepthStencil(backBuff->GetGraphSize());

	m_player = std::make_unique<Player>();
	m_player->Init();

	m_mapModel = std::make_shared<ModelObject>("resource/user/map/", "mapModel.glb");
	m_mapModel->m_transform.SetScale(100.0f);

	m_gameCam = std::make_shared<Camera>(m_gameCamKey);
	GameManager::Instance()->RegisterCamera(m_gameCamKey, m_gameCam);
	GameManager::Instance()->ChangeCamera(m_gameCamKey);

	//敵を生成。
	m_enemyMgr = std::make_shared<EnemyMgr>();
	m_enemyMgr->Init();

	//フィーバーのタイマーを設定。
	m_feverGameTimer = std::make_unique<GameTimer>();
	m_feverGameTimer->Init(m_player->GetMaxFeverTimeGameTimer());

	//弾クラスを生成。
	m_bulletMgr = std::make_shared<BulletMgr>();

	//敵ウェーブ管理クラス
	m_enemyWaveMgr = std::make_unique<EnemyWaveMgr>(MAP_SIZE);

	//エミッシブマップ生成
	m_emissiveMap = D3D12App::Instance()->GenerateRenderTarget(DXGI_FORMAT_R32G32B32A32_FLOAT, Color(0, 0, 0, 1), backBuff->GetGraphSize(), L"EmissiveMap");

	//デプスマップ生成
	m_depthMap = D3D12App::Instance()->GenerateRenderTarget(DXGI_FORMAT_R32_FLOAT, Color(0, 0, 0, 0), backBuff->GetGraphSize(), L"DepthMap");

	//環境マネージャ生成
	m_environmentMgr = std::make_unique<EnvironmentMgr>();

	//m_gameTimer = std::make_unique<GameTimer>();
}

void GameScene::OnInitialize()
{

	/*===== 初期化処理 =====*/
	m_player->Init();
	m_enemyMgr->Init();
	m_bulletMgr->Init();
	m_enemyWaveMgr->Init();

	m_baseEye = Vec3<float>(m_player->GetPos() + Vec3<float>(30.0f, 30.0f, 0.0f));
	m_nowEye = m_baseEye;
	m_baseTarget = m_player->GetPos();
	m_nowTarget = m_baseTarget;

	m_environmentMgr->Init();
}

void GameScene::OnUpdate()
{

	/*===== 更新処理 =====*/

	//スクリーンサイズを取得。
	Vec2<float> windowSize = Vec2<float>(WinApp::Instance()->GetWinSize().x, WinApp::Instance()->GetWinSize().y);

	//現在のカメラ取得
	auto& nowCam = *GameManager::Instance()->GetNowCamera();

	//ゲームマネージャ更新
	GameManager::Instance()->Update();

	// プレイヤー更新処理
	m_player->Update(nowCam, m_bulletMgr, m_enemyMgr, windowSize, MAP_SIZE, EDGE_SCOPE);

	// 敵ウェーブ管理クラスを更新。
	m_enemyWaveMgr->Update(m_enemyMgr, m_player->GetPos(), MAP_SIZE);

	// 敵更新処理
	m_enemyMgr->Update(m_bulletMgr, m_player->GetPos(), MAP_SIZE);

	// 弾を更新。
	m_bulletMgr->Update(MAP_SIZE);


	Vec3<float> playerVecX = -m_player->GetForwardVec();
	const float CAMERA_DISTANCE = 80.0f;

	// カメラを補間。
	m_baseEye = m_player->GetPos() + Vec3<float>(CAMERA_DISTANCE, CAMERA_DISTANCE, 0);
	m_nowEye = m_baseEye;

	m_baseTarget = m_player->GetPos();
	m_baseTarget.x += 3.0f;
	m_nowTarget = m_baseTarget;

	m_gameCam->SetPos(m_nowEye);
	m_gameCam->SetTarget(m_nowTarget);

	m_environmentMgr->Update();

	// フィーバータイムのUIを更新。
	if (m_player->GetIsFever()) {
		m_feverGameTimer->Start();
	}
	else {
		if (m_feverGameTimer->FinishAllEffect()) {
			m_feverGameTimer->Init(m_player->GetMaxFeverTimeGameTimer());
		}
	}
	m_feverGameTimer->Update();

	//if (!m_gameTimer->IsStart() && m_enemyWaveMgr->IsNowWaveBounusStage())
	//{
	//	m_gameTimer->Init(10);
	//	m_gameTimer->Start();
	//}
	//m_gameTimer->Update();



}

void GameScene::OnDraw()
{
	/*===== 描画処理 =====*/

	//デプスステンシルクリア
	KuroEngine::Instance()->Graphics().ClearDepthStencil(m_depthStencil);

	//バックバッファ取得
	auto backBuff = D3D12App::Instance()->GetBackBuffRenderTarget();

	//現在のカメラ取得
	auto& nowCam = GameManager::Instance()->GetNowCamera();

	//DrawFunc初期化
	DrawFunc_Append::FrameInit(
		backBuff,
		m_emissiveMap,
		m_depthMap,
		m_depthStencil,
		nowCam,
		m_environmentMgr->GetLigMgr()
	);

/*--- 通常描画 ---*/
	//環境描画
	m_environmentMgr->Draw(*nowCam);

	//プレイヤー描画
	m_player->Draw(*nowCam);

	//敵を描画
	m_enemyMgr->Draw(nowCam, backBuff, m_emissiveMap, m_depthStencil);

	//弾を描画。
	m_bulletMgr->Draw();



	// マップを描画
	m_mapModel->m_transform.SetScale(MAP_SIZE);
	DrawFunc_Append::DrawModel(m_mapModel);


	// フィーバータイムのUIを描画。
	m_feverGameTimer->Draw();

	//m_gameTimer->Draw();

	/*--- エミッシブマップ合成 ---*/
		//ライトブルームデバイスを使って加算合成
	if (m_emissive)
	{
		m_ligBloomDev.Draw(m_emissiveMap, backBuff);
	}


	/* --- デバッグ描画 ---*/
#ifdef _DEBUG
	//デプスステンシルクリア
	KuroEngine::Instance()->Graphics().ClearDepthStencil(m_depthStencil);
	//レンダーターゲットセット（バックバッファとデプスステンシル）
	KuroEngine::Instance()->Graphics().SetRenderTargets({ backBuff }, m_depthStencil);

	//デバッグ描画フラグ確認
	if (GameManager::Instance()->GetDebugDrawFlg())
	{
		//XYZ軸
		static const float LEN = 100.0f;
		static const float THICKNESS = 0.5f;
		static Vec3<float>ORIGIN = { 0,0,0 };
		DrawFunc3D::DrawLine(*nowCam, ORIGIN, { LEN,0,0 }, Color(1.0f, 0.0f, 0.0f, 1.0f), THICKNESS);
		DrawFunc3D::DrawLine(*nowCam, ORIGIN, { 0,LEN,0 }, Color(0.0f, 1.0f, 0.0f, 1.0f), THICKNESS);
		DrawFunc3D::DrawLine(*nowCam, ORIGIN, { 0,0,LEN }, Color(0.0f, 0.0f, 1.0f, 1.0f), THICKNESS);

		m_player->DrawDebugInfo(*nowCam);

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
	ImGui::End();

	//ゲームマネージャimguiデバッグ
	GameManager::Instance()->ImGuiDebug();
}

void GameScene::OnFinalize()
{
}