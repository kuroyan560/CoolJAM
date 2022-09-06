#include"GameScene.h"
#include"GameManager.h"
#include"DrawFunc3D.h"
#include"Player.h"
#include"Importer.h"

GameScene::GameScene()
{
/*===== コンストラクタ =====*/
	//デプスステンシル生成（バックバッファと同じサイズ）
	auto backBuff = D3D12App::Instance()->GetBackBuffRenderTarget();
	m_depthStencil = D3D12App::Instance()->GenerateDepthStencil(backBuff->GetGraphSize());

	m_player = std::make_unique<Player>();
	m_player->Init();

	m_mapModel = std::make_shared<ModelObject>("resource/user/map/", "mapModel.glb");
	m_mapModel->m_transform.SetScale(100.0f);

	m_gameCam = std::make_shared<Camera>(m_gameCamKey);
	GameManager::Instance()->RegisterCamera(m_gameCamKey, m_gameCam);
	GameManager::Instance()->ChangeCamera(m_gameCamKey);
}

void GameScene::OnInitialize()
{

	/*===== 初期化処理 =====*/
	m_player->Init();
}

void GameScene::OnUpdate()
{

	/*===== 更新処理 =====*/

	GameManager::Instance()->Update();
	m_player->Update(MAP_SIZE, EDGE_SCOPE);

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

	//レンダーターゲットセット（バックバッファとデプスステンシル）
	KuroEngine::Instance()->Graphics().SetRenderTargets({ backBuff }, m_depthStencil);

	//現在のカメラ取得
	auto& nowCam = *GameManager::Instance()->GetNowCamera();

	// マップを描画
	DrawFunc3D::DrawNonShadingModel(m_mapModel, nowCam);

	//プレイヤー描画
	m_player->Draw(nowCam);

	//デバッグ描画
#ifdef _DEBUG
	if (GameManager::Instance()->GetDebugDrawFlg())
	{
		//XYZ軸
		static const float LEN = 100.0f;
		static const float THICKNESS = 0.5f;
		static Vec3<float>ORIGIN = { 0,0,0 };
		DrawFunc3D::DrawLine(nowCam, ORIGIN, { LEN,0,0 }, Color(1.0f, 0.0f, 0.0f, 1.0f), THICKNESS);
		DrawFunc3D::DrawLine(nowCam, ORIGIN, { 0,LEN,0 }, Color(0.0f, 1.0f, 0.0f, 1.0f), THICKNESS);
		DrawFunc3D::DrawLine(nowCam, ORIGIN, { 0,0,LEN }, Color(0.0f, 0.0f, 1.0f, 1.0f), THICKNESS);
	}
#endif
}

void GameScene::OnImguiDebug()
{
	//ゲームマネージャimguiデバッグ
	GameManager::Instance()->ImGuiDebug();
}

void GameScene::OnFinalize()
{
}