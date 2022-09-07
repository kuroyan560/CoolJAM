#include"GameScene.h"
#include"GameManager.h"
#include"DrawFunc3D.h"
#include"Player.h"
#include"Importer.h"
#include"../engine/Common/Angle.h"
#include"KazCollisionHelper.h"
#include"../engine/DrawFuncBillBoard.h"

GameScene::GameScene()
{
	texBuff = D3D12App::Instance()->GenerateTextureBuffer("resource/user/Particle/GrazeParticle.png");


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
	m_grazeEmitter->Init(m_player->GetPosPtr(), m_player->GetInputRadianPtr());

}

void GameScene::OnUpdate()
{
	/*===== 更新処理 =====*/
	GameManager::Instance()->Update();
	m_player->Update(MAP_SIZE, EDGE_SCOPE);

	float distance = MAP_SIZE - m_player->GetPos().Length();
	m_grazeEmitter->Update(MAP_SIZE, distance <= 20.0f);


	//現在のカメラ取得
	auto &nowCam = *GameManager::Instance()->GetNowCamera();

	nowCam.SetPos(m_player->GetPos() + Vec3<float>(30, 30, 0));
	nowCam.SetTarget(m_player->GetPos());
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
	auto &nowCam = *GameManager::Instance()->GetNowCamera();

	// マップを描画
	DrawFunc3D::DrawNonShadingModel(m_mapModel, m_mapModelTransform, nowCam);

	//プレイヤー描画
	m_player->Draw(nowCam);
	m_grazeEmitter->Draw(nowCam);

	float radian = Angle::ConvertToRadian(90);
	float cosRadian = cosf(m_player->GetInputRadian() + radian);
	float sinRadian = sinf(m_player->GetInputRadian() + radian);
	Vec3<float>vel(cosRadian * 100.0f, 0.0f, sinRadian * -100.0f);
	DrawFunc3D::DrawLine(nowCam, m_player->GetPos(),
		m_player->GetPos() + vel
		, Color(255, 0, 255, 255), 1.0f);

	DrawFuncBillBoard::Graph(nowCam, limitPos, { 1.0f,1.0f }, texBuff);

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
	ImGui::Begin("Player");
	ImGui::Text("POS_X:%f", m_player->GetPos().x);
	ImGui::Text("POS_Y:%f", m_player->GetPos().y);
	ImGui::Text("POS_Z:%f", m_player->GetPos().z);
	ImGui::End();
}

void GameScene::OnFinalize()
{
}