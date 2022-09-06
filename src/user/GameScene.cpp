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
	m_grazeEmitter = std::make_unique<GrazeEmitter>();
	m_player->Init();


	m_mapModel = Importer::Instance()->LoadModel("resource/user/map/", "mapModel.glb");
	m_mapModelTransform.SetScale(100.0f);

}

void GameScene::OnInitialize()
{

	/*===== 初期化処理 =====*/

	m_player->Init();
	m_grazeEmitter->Init(m_player->GetPosPtr(),m_player->GetVelPtr());

}

void GameScene::OnUpdate()
{

	/*===== 更新処理 =====*/

	GameManager::Instance()->Update();
	m_player->Update(MAP_SIZE, EDGE_SCOPE);

	//現在のカメラ取得
	auto& nowCam = *GameManager::Instance()->GetNowCamera();

	nowCam.SetPos(m_player->GetPos() + Vec3<float>(30, 30, 0));
	nowCam.SetTarget(m_player->GetPos());

	m_grazeEmitter->Update(MAP_SIZE);

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
	DrawFunc3D::DrawNonShadingModel(m_mapModel, m_mapModelTransform, nowCam);

	//プレイヤー描画
	m_player->Draw(nowCam);
	m_grazeEmitter->Draw(nowCam);

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