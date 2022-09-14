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
#include "GameMode.h"
#include"AudioApp.h"
#include "TitleUI.h"
#include "EnemyMgr.h"
#include "BulletMgr.h"
#include "EnemyWaveMgr.h"
#include"StageFloor.h"

TitleScene::TitleScene()
{

	//バックバッファ情報取得
	auto backBuff = D3D12App::Instance()->GetBackBuffRenderTarget();

	//デプスステンシル生成（バックバッファと同じサイズ）
	m_depthStencil = D3D12App::Instance()->GenerateDepthStencil(backBuff->GetGraphSize());

	m_gameCam = std::make_shared<Camera>(m_gameCamKey);
	GameManager::Instance()->RegisterCamera(m_gameCamKey, m_gameCam);
	GameManager::Instance()->ChangeCamera(m_gameCamKey);

	//敵を生成。
	m_enemyMgr = std::make_shared<EnemyMgr>();
	m_enemyMgr->Init();

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

	// シーン遷移
	m_sceneTransition = std::make_unique<SceneTransition>();

	// プレイヤーを生成。
	m_player = std::make_unique<Player>();
	m_player->Init();

	m_isTransition = false;
	m_transitionEasingTimer = 0;
	m_endEasingTransitionTimer = END_EASING_TIMER;

	// タイトル画像をロード
	m_titleTexture = D3D12App::Instance()->GenerateTextureBuffer("resource/user/UI/title.png");
	m_titleAngle = 0;
	m_isTitle = true;

	// UI用のテクスチャをロード
	m_selectUI[0] = D3D12App::Instance()->GenerateTextureBuffer("resource/user/UI/title_select_tutorial.png");
	m_selectUI[1] = D3D12App::Instance()->GenerateTextureBuffer("resource/user/UI/title_select_Gamel.png");
	m_selectUI[2] = D3D12App::Instance()->GenerateTextureBuffer("resource/user/UI/title_select_Exit.png");

	m_revolverPos = OFF_SCREEN_REVOLVER_POS;

	m_isAppear = false;

	m_nowSelect = SELECT::GAME;

	m_titleUI[0] = std::make_shared<TitleUI>("resource/user/UI/title_select_tutorial.png", TitleUI::STATUS::UP, TitleUI::UI_STATUS::TUTORIAL);
	m_titleUI[1] = std::make_shared<TitleUI>("resource/user/UI/title_select_Gamel.png", TitleUI::STATUS::MIDDLE, TitleUI::UI_STATUS::GAME);
	m_titleUI[2] = std::make_shared<TitleUI>("resource/user/UI/title_select_Exit.png", TitleUI::STATUS::DOWN, TitleUI::UI_STATUS::EXIT);

	m_selectSE = AudioApp::Instance()->LoadAudio("resource/user/sound/select.wav");
	m_enterSE = AudioApp::Instance()->LoadAudio("resource/user/sound/enter.wav");
}

void TitleScene::OnInitialize()
{

	m_environmentMgr->Init();
	m_isTransition = false;
	m_transitionEasingTimer = 0;
	m_endEasingTransitionTimer = END_EASING_TIMER;

	m_revolverPos = OFF_SCREEN_REVOLVER_POS;
	m_revolverEasingTimer = 0;

	m_isTitle = true;
	m_titleAngle = DirectX::XM_PI;
	m_isAppear = false;

	m_nowSelect = SELECT::GAME;

	for (auto& index : m_titleUI) {

		index->Init();

	}

	DrawFunc_Append::RegisterRenderTargets(D3D12App::Instance()->GetBackBuffFormat(), m_emissiveMap, m_depthMap, m_depthStencil);
}

void TitleScene::OnUpdate()
{

	//現在のカメラ取得
	auto& nowCam = *GameManager::Instance()->GetNowCamera();

	//ゲームマネージャ更新
	GameManager::Instance()->Update();

	//スクリーンサイズを取得。
	Vec2<float> windowSize = Vec2<float>(WinApp::Instance()->GetWinSize().x, WinApp::Instance()->GetWinSize().y);
	m_player->Update(nowCam, m_bulletMgr, m_enemyMgr, windowSize, MAP_SIZE, 0, false, true);
	m_player->Init();

	m_environmentMgr->Update(m_player->GetPos());

	// タイトルだったら。
	if (m_isTitle) {

		m_titleAngle -= m_titleAngle / 5.0f;

	}

	// 出現中だったら。
	if (m_isAppear) {

		// イージング量を求める。
		float easingAmount = KuroMath::Ease(Out, Cubic, m_revolverEasingTimer, 0.0f, 1.0f);

		// 位置を求める。
		m_revolverPos = OFF_SCREEN_REVOLVER_POS + (REVOLVER_POS - OFF_SCREEN_REVOLVER_POS) * easingAmount;

		// タイトルを回転させる。
		m_titleAngle = DirectX::XM_2PI - DirectX::XM_PI * easingAmount;

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

		// タイマーを更新。
		m_revolverEasingTimer += ADD_REVOLVER_EASING_TIMER;
		if (1.0f <= m_revolverEasingTimer) {

			m_revolverEasingTimer = 1.0f;

		}

		// UIを回転させる。
		bool canTrans = true;
		for (auto& index : m_titleUI) {

			canTrans &= index->GetCanTrans();

		}
		if (canTrans) {

			for (auto& index : m_titleUI) {

				index->TransDown();

			}

		}

	}

	// 選択中の更新処理
	UpdateSelect();


	// カメラの更新処理
	UpdateCamera();



	for (auto& index : m_titleUI) {

		index->Update();

	}


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
	StageFloor::Instance()->ClearScreen();
	StageFloor::Instance()->Draw();

	// プレイヤーを描画
	m_player->Draw(*nowCam);

	// タイトルを描画。
	Vec2<float> vec = Vec2<float>(cosf(m_titleAngle), sinf(m_titleAngle));
	Vec2<float> centerPos = vec * 380.0f + Vec2<float>(0, 720.0f / 2.0f);
	DrawFunc2D::DrawRotaGraph2D(centerPos, Vec2<float>(1.2f, 1.2f), m_titleAngle, m_titleTexture);


	for (auto& index : m_titleUI) {

		index->Draw(m_revolverPos);

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


	if (UsersInput::Instance()->MouseOnTrigger(LEFT)) {

		AudioApp::Instance()->PlayWaveDelay(m_enterSE);

		// 出現中じゃなかったら。
		if (!m_isTitle && !m_isAppear && !m_isTransition) {

			m_isTransition = true;

			m_revolverEasingTimer = 0;

			if (m_nowSelect == SELECT::GAME) {
				GameMode::Instance()->m_id = GameMode::ID::GAME;
			}
			if (m_nowSelect == SELECT::TUTORIAL) {
				GameMode::Instance()->m_id = GameMode::ID::TUTORIAL;
			}

			if (m_nowSelect == SELECT::EXIT) {

				KuroEngine::Instance()->GameEnd();

			}

		}
		// タイトル描画中だったら。
		if (m_isTitle) {

			m_isAppear = true;
			m_isTitle = false;
		}

	}

	// 遷移可能か
	bool canTrans = true;
	for (auto& index : m_titleUI) {

		canTrans &= index->GetCanTrans();

	}
	auto oldSelect = m_nowSelect;

	float mouseMove = UsersInput::Instance()->GetMouseMove().m_inputZ;
	if (mouseMove < -100.0f && !m_isAppear && !m_isTransition && !m_isTitle && canTrans) {

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

		for (auto& index : m_titleUI) {

			index->TransUp();

		}

	}

	if (100.0f < mouseMove && !m_isAppear && !m_isTransition && !m_isTitle && canTrans) {

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

		for (auto& index : m_titleUI) {

			index->TransDown();

		}

	}

	if (oldSelect != m_nowSelect)AudioApp::Instance()->PlayWaveDelay(m_selectSE);

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

				// 遷移先のシーンを決める。
				int transitionSceneNum = 0;
				if (GameMode::Instance()->m_id == GameMode::ID::GAME) {
					transitionSceneNum = 2;
				}
				else if (GameMode::Instance()->m_id == GameMode::ID::TUTORIAL) {
					transitionSceneNum = 1;
				}

				KuroEngine::Instance()->ChangeScene(transitionSceneNum, m_sceneTransition.get());

			}

		}

		// 補間先注視点座標
		bool isGame = GameMode::Instance()->m_id == GameMode::ID::GAME;
		Vec3<float> endTarget = (isGame ? END_GAME_TARGET_POS : END_TUTORIAL_TARGET_POS);
		Vec3<float> endEye = (isGame ? END_GAME_EYE_POS : END_TUTORIAL_EYE_POS);
		float endLength = (isGame ? END_GAME_LENGTH : END_TUTORIAL_LENGTH);

		// イージング量を求める。
		float easingAmount = KuroMath::Ease(InOut, Cubic, m_transitionEasingTimer, 0.0f, 1.0f);

		// 注視点の高さを求める。
		Vec3<float> nowTargetPos = nowCam.GetTarget();
		nowTargetPos = DEF_TARGET_POS + (endTarget - DEF_TARGET_POS) * easingAmount;
		nowCam.SetTarget(nowTargetPos);

		// 視点の高さを求める。
		float nowEyeHeight = DEF_EYE_POS.y + (endEye.y - DEF_EYE_POS.y) * easingAmount;

		// 現在の角度。
		float nowAngle = DEF_ANGLE + (END_ANGLE - DEF_ANGLE) * easingAmount;

		// 現在の長さ。
		float nowLength = DEF_LENGTH + (endLength - DEF_LENGTH) * easingAmount;

		// 現在の視点座標を求める。
		Vec3<float> nowEyePos = Vec3<float>(cosf(nowAngle) * nowLength, nowEyeHeight, sinf(nowAngle) * nowLength);
		nowCam.SetPos(nowEyePos);

	}
	else {


		// イージング量を求める。 遷移中とのズレを無くすために
		float easingAmount = 0;

		// 注視点の高さを求める。
		Vec3<float> nowTargetPos = nowCam.GetTarget();
		nowTargetPos = DEF_TARGET_POS + (END_GAME_TARGET_POS - DEF_TARGET_POS) * easingAmount;
		nowCam.SetTarget(nowTargetPos);

		// 視点の高さを求める。
		float nowEyeHeight = DEF_EYE_POS.y + (END_GAME_EYE_POS.y - DEF_EYE_POS.y) * easingAmount;

		// 現在の角度。
		float nowAngle = DEF_ANGLE + CHANGE_ANGLE * easingAmount;

		// 現在の長さ。
		float nowLength = DEF_LENGTH + (END_GAME_LENGTH - DEF_LENGTH) * easingAmount;

		// 現在の視点座標を求める。
		Vec3<float> nowEyePos = Vec3<float>(cosf(nowAngle) * nowLength, nowEyeHeight, sinf(nowAngle) * nowLength);
		nowCam.SetPos(nowEyePos);

		m_endEasingTransitionTimer = END_EASING_TIMER;

	}


}