#include "GameScene.h"
#include"GameManager.h"
#include"DrawFunc3D.h"
#include"DrawFunc2D.h"

GameScene::GameScene()
{
	//バックバッファ情報取得
	auto backBuff = D3D12App::Instance()->GetBackBuffRenderTarget();

	//メインレンダーターゲット
	m_mainTarget = D3D12App::Instance()->GenerateRenderTarget(DXGI_FORMAT_R32G32B32A32_FLOAT, Color(0, 0, 0, 0), backBuff->GetGraphSize(), L"MainTarget");

	//デプスステンシル生成（バックバッファと同じサイズ）
	m_depthStencil = D3D12App::Instance()->GenerateDepthStencil(backBuff->GetGraphSize());

	//エミッシブマップ生成
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
	//バックバッファ取得
	auto backBuff = D3D12App::Instance()->GetBackBuffRenderTarget();

	//現在のカメラ取得
	auto& nowCam = *GameManager::Instance()->GetNowCamera();


/*--- 通常描画 ---*/
	//デプスステンシルクリア
	KuroEngine::Instance()->Graphics().ClearDepthStencil(m_depthStencil);
	//レンダーターゲットセット（バックバッファとデプスステンシル）
	KuroEngine::Instance()->Graphics().SetRenderTargets({ m_mainTarget }, m_depthStencil);

	//プレイヤー描画
	m_player.Draw(nowCam);

/*--- エミッシブマップに描画 ---*/
	//デプスステンシルクリア
	KuroEngine::Instance()->Graphics().ClearDepthStencil(m_depthStencil);
	//エミッシブマップクリア
	KuroEngine::Instance()->Graphics().ClearRenderTarget(m_emissiveMap);
	//レンダーターゲットセット（エミッシブマップとデプスステンシル）
	KuroEngine::Instance()->Graphics().SetRenderTargets({ m_emissiveMap }, m_depthStencil);

	//プレイヤー描画
	m_player.Draw(nowCam);

/*--- エミッシブマップ合成 ---*/
	//ライトブルームデバイスを使って加算合成
	m_ligBloomDev.Draw(m_emissiveMap, m_mainTarget);

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
	//ゲームマネージャimguiデバッグ
	GameManager::Instance()->ImGuiDebug();
}

void GameScene::OnFinalize()
{
}