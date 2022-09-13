#include"TutorialScene.h"
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
#include"Tutorial.h"
#include"StageFloor.h"

TutorialScene::TutorialScene()
{
	//バックバッファ情報取得
	auto backBuff = D3D12App::Instance()->GetBackBuffRenderTarget();

	/*===== コンストラクタ =====*/

	//デプスステンシル生成（バックバッファと同じサイズ）
	m_depthStencil = D3D12App::Instance()->GenerateDepthStencil(backBuff->GetGraphSize());

	m_player = std::make_shared<Player>();
	m_grazeEmitter = std::make_unique<GrazeEmitter>();
	m_player->Init();

	m_gameCam = std::make_shared<Camera>(m_gameCamKey);
	GameManager::Instance()->RegisterCamera(m_gameCamKey, m_gameCam);
	GameManager::Instance()->ChangeCamera(m_gameCamKey);

	//敵を生成。
	m_enemyMgr = std::make_shared<EnemyMgr>();
	m_enemyMgr->Init();

	//弾クラスを生成。
	m_bulletMgr = std::make_shared<BulletMgr>();

	// フィーバーゲージ
	m_feverGauge = std::make_unique<FeverGauge>();

	// シーン遷移
	m_sceneTransition = std::make_unique<SceneTransition>();

	//敵ウェーブ管理クラス
	m_enemyWaveMgr = std::make_unique<EnemyWaveMgr>(MAP_SIZE);

	//エミッシブマップ生成
	m_emissiveMap = D3D12App::Instance()->GenerateRenderTarget(DXGI_FORMAT_R32G32B32A32_FLOAT, Color(0, 0, 0, 1), backBuff->GetGraphSize(), L"EmissiveMap");

	//デプスマップ生成
	m_depthMap = D3D12App::Instance()->GenerateRenderTarget(DXGI_FORMAT_R32_FLOAT, Color(0, 0, 0, 0), backBuff->GetGraphSize(), L"DepthMap");

	//環境マネージャ生成
	m_environmentMgr = std::make_unique<EnvironmentMgr>();

	const auto backBuffFormat = backBuff->GetDesc().Format;
	DrawFunc_Append::RegisterRenderTargets(backBuffFormat, m_emissiveMap, m_depthMap, m_depthStencil);
	DrawFunc3D::GenerateDrawLinePipeline(backBuffFormat);
	DrawFunc3D::GenerateDrawLinePipeline(backBuffFormat, AlphaBlendMode_Add);

	m_isCameraHomePosition = true;
	m_isTransitionStart = false;
	m_isInitPlayer = false;
	m_isCompleteUpper = false;
	m_returnTitlePosEasingTimer = 0;
	m_transitionDelayTimer = 0;

	m_tutorial = std::make_shared<Tutorial>();
}

void TutorialScene::OnInitialize()
{

	/*===== 初期化処理 =====*/
	m_player->Init();
	m_enemyMgr->Init();
	m_bulletMgr->Init();
	m_enemyWaveMgr->Init(60);
	m_feverGauge->Init();

	m_environmentMgr->Init();
	m_player->Init();
	m_grazeEmitter->Init(m_player->GetPosPtr(), m_player->GetInputRadianPtr());

	m_isCameraHomePosition = true;
	m_isTransitionStart = false;
	m_isInitPlayer = false;
	m_isCompleteUpper = false;
	m_returnTitlePosEasingTimer = 0;
	m_transitionDelayTimer = 0;

	m_tutorial->Init();

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

}

void TutorialScene::OnUpdate()
{
	/*===== 更新処理 =====*/

	//現在のカメラ取得
	auto& nowCam = *GameManager::Instance()->GetNowCamera();



	//スクリーンサイズを取得。
	Vec2<float> windowSize = Vec2<float>(WinApp::Instance()->GetWinSize().x, WinApp::Instance()->GetWinSize().y);

	m_grazeEmitter->Update(MAP_SIZE);

	//ゲームマネージャ更新
	GameManager::Instance()->Update();

	// プレイヤー更新処理
	m_player->Update(nowCam, m_bulletMgr, m_enemyMgr, windowSize, MAP_SIZE, EDGE_SCOPE, m_tutorial->GetPlayerFeverGaugeStop(m_enemyMgr));

	// 遷移が始まったらプレイヤーを常に初期化し続ける。
	if (m_isInitPlayer) {

		m_player->Init();

		m_tutorial->Init();
	}


	// 敵更新処理
	m_enemyMgr->Update(m_bulletMgr, m_player->GetPos(), MAP_SIZE);

	// 弾を更新。
	m_bulletMgr->Update(MAP_SIZE);

	// チュートリアルのアップデート
	m_tutorial->Update(m_player, m_enemyMgr, m_bulletMgr, m_isCameraHomePosition, MAP_SIZE, m_isTransitionStart);

	// ゲームの状態に応じてカメラの位置を変える。
	if (m_isTransitionStart) {

		// 上を向ききっていたら。
		if (m_isCompleteUpper) {

			// カメラをタイトルのカメラの位置まで持っていくイージングのタイマーを更新。
			m_returnTitlePosEasingTimer += 0.03f;
			if (1.0f <= m_returnTitlePosEasingTimer) m_returnTitlePosEasingTimer = 1.0f;
			float easingAmount = KuroMath::Ease(InOut, Exp, m_returnTitlePosEasingTimer, 0.0f, 1.0f);

			// カメラの位置をセット。
			nowCam.SetTarget(m_baseEasingCameraTarget + (TITLE_TARGET_POS - m_baseEasingCameraTarget) * easingAmount);
			nowCam.SetPos(m_baseEasingCameraEye + (TITLE_EYE_POS - m_baseEasingCameraEye) * easingAmount);


			// 補間先との距離が一定以下になったら遷移させる。
			if (1.0f <= m_returnTitlePosEasingTimer) {

				++m_transitionDelayTimer;
				if (30 < m_transitionDelayTimer) {
					KuroEngine::Instance()->ChangeScene(0, m_sceneTransition.get());

				}
			}

		}
		else {

			Vec3<float> nowCameraTarget = nowCam.GetTarget();

			nowCameraTarget.y += (TRANSITION_CAMERA_Y - nowCameraTarget.y) / 5.0f;

			nowCam.SetTarget(nowCameraTarget);

			// 補間先までの距離が一定以下になったら次へ
			if (fabs(nowCameraTarget.y - TRANSITION_CAMERA_Y) <= 1.0f) {

				m_isInitPlayer = true;
				m_isCompleteUpper = true;

				m_baseEasingCameraEye = nowCam.GetPos();
				m_baseEasingCameraTarget = nowCam.GetTarget();

			}

		}


	}
	else if (m_isCameraHomePosition) {

		m_nowEye += (CAMERA_HOME_EYE_POSITION - m_nowEye) / 5.0f;
		m_nowTarget += (CAMERA_HOME_TARGET_POSITION - m_nowTarget) / 5.0f;
		m_gameCam->SetPos(m_nowEye);
		m_gameCam->SetTarget(m_nowTarget);

	}
	else {

		Vec3<float> playerVecX = -m_player->GetForwardVec();
		const float CAMERA_DISTANCE = 80.0f;

		// カメラを補間。
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

	// チュートリアル状態の時、エンターキーを押すことでゲームモードのカメラに移行する。
	if (UsersInput::Instance()->KeyInput(DIK_RETURN)) {
		m_isCameraHomePosition = true;
	}
	if (UsersInput::Instance()->KeyInput(DIK_SPACE)) {
		m_isCameraHomePosition = false;
	}

	m_environmentMgr->Update(m_player->GetPos());

	m_feverGauge->Update(m_player->GetIsFever(), m_player->GetPlayerFeverRate());

}

#include "DrawFunc2D.h"
void TutorialScene::OnDraw()
{
	/*===== 描画処理 =====*/

	//バックバッファ取得
	auto backBuff = D3D12App::Instance()->GetBackBuffRenderTarget();

	//現在のカメラ取得
	GameManager::Instance()->ChangeCamera(m_gameCamKey);
	auto& nowCam = GameManager::Instance()->GetNowCamera();

	//DrawFunc初期化
	DrawFunc_Append::FrameInit(
		backBuff,
		nowCam,
		m_environmentMgr->GetLigMgr()
	);

	KuroEngine::Instance()->Graphics().ClearRenderTarget(m_emissiveMap);

	/*--- 通常描画 ---*/

	// チュートリアルの描画
	m_tutorial->Draw();

	// マップを描画
	StageFloor::Instance()->Draw();

	//環境描画
	m_environmentMgr->Draw(*nowCam);

	//プレイヤー描画
	m_player->Draw(*nowCam);

	//敵を描画
	m_enemyMgr->Draw(*nowCam, backBuff, m_emissiveMap, m_depthStencil);

	//弾を描画。
	m_bulletMgr->Draw();

	// チュートリアルの戻るのアイコンを描画
	m_tutorial->DrawReturnIcon();

	// フィーバーゲージを描画。
	m_feverGauge->Draw();

	m_grazeEmitter->Draw(*nowCam);

	float radian = Angle::ConvertToRadian(90);
	float cosRadian = cosf(m_player->GetInputRadian() + radian);
	float sinRadian = sinf(m_player->GetInputRadian() + radian);
	Vec3<float>vel(cosRadian * 100.0f, 0.0f, sinRadian * -100.0f);
	//DrawFunc3D::DrawLine(*nowCam, m_player->GetPos(),
	//	m_player->GetPos() + vel
	//	, Color(255, 0, 255, 255), 1.0f);


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

void TutorialScene::OnImguiDebug()
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
	ImGui::Begin("Player");
	ImGui::Text("POS_X:%f", m_player->GetPos().x);
	ImGui::Text("POS_Y:%f", m_player->GetPos().y);
	ImGui::Text("POS_Z:%f", m_player->GetPos().z);
	ImGui::End();
}

void TutorialScene::OnFinalize()
{
}