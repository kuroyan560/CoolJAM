#include "TitleScene.h"
#include "Player.h"
#include "Camera.h"
#include "GameManager.h"
#include "EnvironmentMgr.h"
#include "DrawFunc_Append.h"
#include "DrawFunc3D.h"

TitleScene::TitleScene()
{

	//バックバッファ情報取得
	auto backBuff = D3D12App::Instance()->GetBackBuffRenderTarget();

	//デプスステンシル生成（バックバッファと同じサイズ）
	m_depthStencil = D3D12App::Instance()->GenerateDepthStencil(backBuff->GetGraphSize());

	m_gameCam = std::make_shared<Camera>(m_gameCamKey);
	GameManager::Instance()->RegisterCamera(m_gameCamKey, m_gameCam);
	GameManager::Instance()->ChangeCamera(m_gameCamKey);

	//エミッシブマップ生成
	m_emissiveMap = D3D12App::Instance()->GenerateRenderTarget(DXGI_FORMAT_R32G32B32A32_FLOAT, Color(0, 0, 0, 1), backBuff->GetGraphSize(), L"EmissiveMap");

	//デプスマップ生成
	m_depthMap = D3D12App::Instance()->GenerateRenderTarget(DXGI_FORMAT_R32_FLOAT, Color(0, 0, 0, 0), backBuff->GetGraphSize(), L"DepthMap");

	//環境マネージャ生成
	m_environmentMgr = std::make_unique<EnvironmentMgr>();

	// シーン遷移
	m_sceneTransition = std::make_unique<SceneTransition>();

	// プレイヤーを生成。
	m_player = std::make_unique<Player>();
	m_player->Init();

	m_isTransition = false;
	m_transitionEasingTimer = 0;
	m_endEasingTransitionTimer = END_EASING_TIMER;

}

void TitleScene::OnInitialize()
{

	m_environmentMgr->Init();
	m_isTransition = false;
	m_transitionEasingTimer = 0;
	m_endEasingTransitionTimer = END_EASING_TIMER;

}

void TitleScene::OnUpdate()
{

	//現在のカメラ取得
	auto& nowCam = *GameManager::Instance()->GetNowCamera();

	//ゲームマネージャ更新
	GameManager::Instance()->Update();

	m_environmentMgr->Update();

	if (UsersInput::Instance()->KeyInput(DIK_SPACE)) {

		m_isTransition = true;
		//KuroEngine::Instance()->ChangeScene(1, m_sceneTransition.get());

	}

	// 遷移中だったら。
	if (m_isTransition) {

		m_transitionEasingTimer += ADD_TRANSITION_EASING_TIMER;
		if (1.0f < m_transitionEasingTimer) {

			m_transitionEasingTimer = 1.0f;

			// イージング後の遷移までのタイマーを更新する。
			--m_endEasingTransitionTimer;
			if (m_endEasingTransitionTimer <= 0) {

				KuroEngine::Instance()->ChangeScene(1, m_sceneTransition.get());

			}

		}

		// イージング量を求める。
		float easingAmount = KuroMath::Ease(InOut, Cubic, m_transitionEasingTimer, 0.0f, 1.0f);

		// 注視点の高さを求める。
		Vec3<float> nowTargetPos = nowCam.GetTarget();
		nowTargetPos = DEF_TARGET_POS + (END_TARGET_POS - DEF_TARGET_POS) * easingAmount;
		nowCam.SetTarget(nowTargetPos);

		// 視点の高さを求める。
		float nowEyeHeight = DEF_EYE_POS.y + (END_EYE_POS.y - DEF_EYE_POS.y) * easingAmount;

		// 現在の角度。
		float nowAngle = DEF_ANGLE + (END_ANGLE - DEF_ANGLE) * easingAmount;

		// 現在の長さ。
		float nowLength = DEF_LENGTH + (END_LENGTH - DEF_LENGTH) * easingAmount;

		// 現在の視点座標を求める。
		Vec3<float> nowEyePos = Vec3<float>(cosf(nowAngle) * nowLength, nowEyeHeight, sinf(nowAngle) * nowLength);
		nowCam.SetPos(nowEyePos);

	}
	else {


		// イージング量を求める。 遷移中とのズレを無くすために
		float easingAmount = 0;

		// 注視点の高さを求める。
		Vec3<float> nowTargetPos = nowCam.GetTarget();
		nowTargetPos = DEF_TARGET_POS + (END_TARGET_POS - DEF_TARGET_POS) * easingAmount;
		nowCam.SetTarget(nowTargetPos);

		// 視点の高さを求める。
		float nowEyeHeight = DEF_EYE_POS.y + (END_EYE_POS.y - DEF_EYE_POS.y) * easingAmount;

		// 現在の角度。
		float nowAngle = DEF_ANGLE + CHANGE_ANGLE * easingAmount;

		// 現在の長さ。
		float nowLength = DEF_LENGTH + (END_LENGTH - DEF_LENGTH) * easingAmount;

		// 現在の視点座標を求める。
		Vec3<float> nowEyePos = Vec3<float>(cosf(nowAngle) * nowLength, nowEyeHeight, sinf(nowAngle) * nowLength);
		nowCam.SetPos(nowEyePos);

		m_endEasingTransitionTimer = END_EASING_TIMER;

	}

}

void TitleScene::OnDraw()
{

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

	//環境描画
	m_environmentMgr->Draw(*nowCam);


	// プレイヤーを描画
	m_player->Draw(*nowCam, true);



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

void TitleScene::OnImguiDebug()
{
	ImGui::Begin("Test");
	ImGui::Checkbox("Emissive", &m_emissive);
	ImGui::End();

	//ゲームマネージャimguiデバッグ
	GameManager::Instance()->ImGuiDebug();
}

void TitleScene::OnFinalize()
{
}
