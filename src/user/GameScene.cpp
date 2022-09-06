#include"GameScene.h"
#include"GameManager.h"
#include"DrawFunc3D.h"
#include"Player.h"
#include"Importer.h"
#include"DrawFunc2D.h"
#include"EnemyMgr.h"
#include"BulletMgr.h"

GameScene::GameScene()
{
	//バックバッファ情報取得
	auto backBuff = D3D12App::Instance()->GetBackBuffRenderTarget();

	//メインレンダーターゲット
	m_mainTarget = D3D12App::Instance()->GenerateRenderTarget(DXGI_FORMAT_R32G32B32A32_FLOAT, Color(23, 14, 41, 255), backBuff->GetGraphSize(), L"MainTarget");

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

	//弾クラスを生成。
	m_bulletMgr = std::make_shared<BulletMgr>();

	//エミッシブマップ生成
	m_emissiveMap = D3D12App::Instance()->GenerateRenderTarget(DXGI_FORMAT_R32G32B32A32_FLOAT, Color(0, 0, 0, 1), backBuff->GetGraphSize(), L"EmissiveMap");
}

void GameScene::OnInitialize()
{

	/*===== 初期化処理 =====*/
	m_player->Init();
	m_enemyMgr->Init();
	m_bulletMgr->Init();

}

void GameScene::OnUpdate()
{

	/*===== 更新処理 =====*/

	GameManager::Instance()->Update();

	// プレイヤー更新処理
	m_player->Update(m_bulletMgr, m_enemyMgr, MAP_SIZE, EDGE_SCOPE);

	// 敵更新処理
	m_enemyMgr->Update(m_bulletMgr, m_player->GetPos(), MAP_SIZE);

	// 弾を更新。
	m_bulletMgr->Update(MAP_SIZE);

	m_gameCam->SetPos(m_player->GetPos() + Vec3<float>(30, 30, 0));
	m_gameCam->SetTarget(m_player->GetPos());

}

void GameScene::OnDraw()
{

	/*===== 描画処理 =====*/

	//デプスステンシルクリア
	KuroEngine::Instance()->Graphics().ClearDepthStencil(m_depthStencil);

	//バックバッファ取得
	auto backBuff = D3D12App::Instance()->GetBackBuffRenderTarget();

	//現在のカメラ取得
	auto& nowCam = *GameManager::Instance()->GetNowCamera();

	//メインレンダーターゲットクリア
	KuroEngine::Instance()->Graphics().ClearRenderTarget(m_mainTarget);


	/*--- 通常描画 ---*/
	//デプスステンシルクリア
	KuroEngine::Instance()->Graphics().ClearDepthStencil(m_depthStencil);
	//レンダーターゲットセット（バックバッファとデプスステンシル）
	KuroEngine::Instance()->Graphics().SetRenderTargets({ m_mainTarget }, m_depthStencil);

	//プレイヤー描画
	m_player->Draw(nowCam);

	//敵を描画
	m_enemyMgr->Draw(nowCam);

	//弾を描画。
	m_bulletMgr->Draw(nowCam);

	// マップを描画
	DrawFunc3D::DrawNonShadingModel(m_mapModel, nowCam);

	/*--- エミッシブマップに描画 ---*/
		//デプスステンシルクリア
	KuroEngine::Instance()->Graphics().ClearDepthStencil(m_depthStencil);
	//エミッシブマップクリア
	KuroEngine::Instance()->Graphics().ClearRenderTarget(m_emissiveMap);
	//レンダーターゲットセット（エミッシブマップとデプスステンシル）
	KuroEngine::Instance()->Graphics().SetRenderTargets({ m_emissiveMap }, m_depthStencil);

	//プレイヤー描画
	m_player->Draw(nowCam);

	/*--- エミッシブマップ合成 ---*/
		//ライトブルームデバイスを使って加算合成
	if (m_emissive)
	{
		m_ligBloomDev.Draw(m_emissiveMap, m_mainTarget);
	}

	/*--- バックバッファに映す ---*/
	KuroEngine::Instance()->Graphics().SetRenderTargets({ backBuff });
	DrawFunc2D::DrawGraph({ 0,0 }, m_mainTarget);

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
	ImGui::Checkbox("PlayerMode", &m_player->m_isDebugParam);
	ImGui::End();

	//ゲームマネージャimguiデバッグ
	GameManager::Instance()->ImGuiDebug();
}

void GameScene::OnFinalize()
{
}