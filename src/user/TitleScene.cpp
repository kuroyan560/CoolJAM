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

	//バックバッファ情報取得
	auto backBuff = D3D12App::Instance()->GetBackBuffRenderTarget();

	//デプスステンシル生成（バックバッファと同じサイズ）
	m_depthStencil = D3D12App::Instance()->GenerateDepthStencil(backBuff->GetGraphSize());

	m_gameCam = std::make_shared<Camera>(m_gameCamKey);
	GameManager::Instance()->RegisterCamera(m_gameCamKey, m_gameCam);
	GameManager::Instance()->ChangeCamera(m_gameCamKey);

	m_mapModel = Importer::Instance()->LoadModel("resource/user/map/", "mapModel.glb");
	m_mapModelTransform.SetScale(150.0f);

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

	// UI用のテクスチャをロード
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

	//現在のカメラ取得
	auto& nowCam = *GameManager::Instance()->GetNowCamera();

	//ゲームマネージャ更新
	GameManager::Instance()->Update();

	m_environmentMgr->Update(m_player->GetPos());

	// 出現中だったら。
	if (m_isAppear) {

		// イージング量を求める。
		float easingAmount = KuroMath::Ease(Out, Cubic, m_revolverEasingTimer, 0.0f, 1.0f);

		// 位置を求める。
		m_revolverPos = OFF_SCREEN_REVOLVER_POS + (REVOLVER_POS - OFF_SCREEN_REVOLVER_POS) * easingAmount;

		// 回転を求める。
		m_rotateUI[0] = OFF_SCREEN_ROTATE[0] + (DEF_ROTATE[0] - OFF_SCREEN_ROTATE[0]) * easingAmount;
		m_rotateUI[1] = OFF_SCREEN_ROTATE[1] + (DEF_ROTATE[1] - OFF_SCREEN_ROTATE[1]) * easingAmount;
		m_rotateUI[2] = OFF_SCREEN_ROTATE[2] + (DEF_ROTATE[2] - OFF_SCREEN_ROTATE[2]) * easingAmount;

		// タイマーを更新。
		m_revolverEasingTimer += ADD_REVOLVER_EASING_TIMER;
		if (1.0f <= m_revolverEasingTimer) {

			m_revolverEasingTimer = 1.0f;
			m_isAppear = false;

		}

	}
	// 遷移中だったら。
	else if (m_isTransition) {

		// イージング量を求める。
		float easingAmount = KuroMath::Ease(In, Cubic, m_revolverEasingTimer, 0.0f, 1.0f);

		// 位置を求める。
		m_revolverPos = REVOLVER_POS + (OFF_SCREEN_REVOLVER_POS - REVOLVER_POS) * easingAmount;

		// 回転を求める。
		m_rotateUI[0] = DEF_ROTATE[0] + (OFF_SCREEN_ROTATE[0] - DEF_ROTATE[0]) * easingAmount;
		m_rotateUI[1] = DEF_ROTATE[1] + (OFF_SCREEN_ROTATE[1] - DEF_ROTATE[1]) * easingAmount;
		m_rotateUI[2] = DEF_ROTATE[2] + (OFF_SCREEN_ROTATE[2] - DEF_ROTATE[2]) * easingAmount;

		// タイマーを更新。
		m_revolverEasingTimer += ADD_REVOLVER_EASING_TIMER;
		if (1.0f <= m_revolverEasingTimer) {

			m_revolverEasingTimer = 1.0f;

		}

	}

	// 選択中の更新処理
	UpdateSelect();


	// カメラの更新処理
	UpdateCamera();


}

#include "DrawFunc2D.h"
#include "DrawFunc3D.h"
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
		nowCam,
		m_environmentMgr->GetLigMgr()
	);

	//環境描画
	m_environmentMgr->Draw(*nowCam);

	// マップを描画
	DrawFunc_Append::DrawModel(m_mapModel, m_mapModelTransform);

	// プレイヤーを描画
	m_player->Draw(*nowCam, true);

	// UI用を描画
	for (auto& index : m_selectUI) {

		// ベクトルを求める。
		Vec2<float> vec = Vec2<float>(cosf(m_rotateUI[static_cast<int>(&index - &m_selectUI[0])]), sinf(m_rotateUI[static_cast<int>(&index - &m_selectUI[0])]));

		Vec2<float> centerPos = m_revolverPos + vec * 300.0f;

		DrawFunc2D::DrawRotaGraph2D(centerPos, Vec2<float>(1.0f, 1.0f), m_rotateUI[static_cast<int>(&index - &m_selectUI[0])], index);

	}


	/*--- エミッシブマップ合成 ---*/
	//ライトブルームデバイスを使って加算合成
	if (m_emissive)
	{
		m_ligBloomDev.Draw(m_emissiveMap, backBuff);
	}


	//	/* --- デバッグ描画 ---*/
	//#ifdef _DEBUG
	//	//デプスステンシルクリア
	//	KuroEngine::Instance()->Graphics().ClearDepthStencil(m_depthStencil);
	//	//レンダーターゲットセット（バックバッファとデプスステンシル）
	//	KuroEngine::Instance()->Graphics().SetRenderTargets({ backBuff }, m_depthStencil);
	//
	//	//デバッグ描画フラグ確認
	//	if (GameManager::Instance()->GetDebugDrawFlg())
	//	{
	//		//XYZ軸
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

	//ゲームマネージャimguiデバッグ
	GameManager::Instance()->ImGuiDebug();
}

void TitleScene::OnFinalize()
{
}

void TitleScene::UpdateSelect() {


	if ((UsersInput::Instance()->KeyInput(DIK_SPACE) || UsersInput::Instance()->KeyInput(DIK_RETURN)) && !m_isAppear) {

		m_isTransition = true;

		m_revolverEasingTimer = 0;
		//KuroEngine::Instance()->ChangeScene(1, m_sceneTransition.get());

		if (m_nowSelect == SELECT::EXIT) {

			exit(0);

		}

	}

	if (UsersInput::Instance()->KeyOnTrigger(DIK_UP) && !m_isAppear && !m_isTransition) {

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
	if (UsersInput::Instance()->KeyOnTrigger(DIK_DOWN) && !m_isAppear && !m_isTransition) {

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

			// 角度がスキップ領域に入っていたら。
			if (MIN_SKIP_ROTATE <= index && index < MAX_SKIP_ROTATE) {
				index = MAX_SKIP_ROTATE + (index - MIN_SKIP_ROTATE);
			}
			// 最高値を超えていたら。
			if (DirectX::XM_2PI <= index) {
				index = index - DirectX::XM_2PI;
			}

		}
		else if (rotate < 0) {

			// 角度がスキップ領域に入っていたら。
			if (index <= MAX_SKIP_ROTATE && MIN_SKIP_ROTATE < index) {
				index = MIN_SKIP_ROTATE - (MAX_SKIP_ROTATE - index);
			}
			// 最低値を超えていたら。
			if (index <= 0) {
				index = DirectX::XM_2PI + index;
			}

		}

	}



}

void TitleScene::UpdateCamera() {

	//現在のカメラ取得
	auto& nowCam = *GameManager::Instance()->GetNowCamera();

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