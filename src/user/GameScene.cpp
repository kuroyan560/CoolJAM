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
#include"../engine/Common/Angle.h"
#include"KazCollisionHelper.h"
#include"../engine/DrawFuncBillBoard.h"
#include"FeverGauge.h"
#include"GameMode.h"
#include"EnemyWaveEditor.h"
#include"StageFloor.h"
#include"Font.h"
#include"ScoreMgr.h"
#include"SlowMgr.h"
#include"ShakeMgr.h"

GameScene::GameScene()
{
	//バックバッファ情報取得
	auto backBuff = D3D12App::Instance()->GetBackBuffRenderTarget();

	/*===== コンストラクタ =====*/

	//デプスステンシル生成（バックバッファと同じサイズ）
	m_depthStencil = D3D12App::Instance()->GenerateDepthStencil(backBuff->GetGraphSize());

	m_player = std::make_unique<Player>();
	m_player->Init();

	m_gameCam = std::make_shared<Camera>(m_gameCamKey);
	GameManager::Instance()->RegisterCamera(m_gameCamKey, m_gameCam);

	//敵を生成。
	m_enemyMgr = std::make_shared<EnemyMgr>();
	m_enemyMgr->Init();

	//弾クラスを生成。
	m_bulletMgr = std::make_shared<BulletMgr>();

	// シーン遷移
	m_sceneTransition = std::make_unique<SceneTransition>();

	// フィーバーゲージ
	m_feverGauge = std::make_unique<FeverGauge>();

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

	//BGM読み込み
	m_bgm = AudioApp::Instance()->LoadAudio("resource/user/sound/bgm.wav", 0.1f);

	Font::Instance()->LoadFont();

	m_gameUI = std::make_unique<GameUI>();

	ScoreMgr::Instance()->Init();

	// 戻るのアイコンを表示
	m_returnTexture = D3D12App::Instance()->GenerateTextureBuffer("resource/user/tutorial/return.png");
	m_returnIconSize = Vec2<float>();
	m_isNearReturnIcon = false;

	m_isStartTransition = false;
	m_isCompleteUpper = false;
	m_returnTitlePosEasingTimer = 0;
	m_transitionDelayTimer = 0;


	m_bonusEffect = std::make_unique<BonusEffect>();

	SlowMgr::Instance()->Init();

	m_isFeverCameraEffect = false;
	m_feverNearCameraTimer = 0;

	ShakeMgr::Instance()->Init();

}

void GameScene::OnInitialize()
{

	/*===== 初期化処理 =====*/
	m_player->Init();
	m_enemyMgr->Init();
	m_bulletMgr->Init();
	m_enemyWaveMgr->Init(60);
	m_feverGauge->Init();

	ScoreMgr::Instance()->Init();

	m_environmentMgr->Init();
	m_player->Init();

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

	AudioApp::Instance()->PlayWave(m_bgm, true);
	GameManager::Instance()->ChangeCamera(m_gameCamKey);

	m_gameUI->Init();
	m_returnIconSize = Vec2<float>();
	m_isNearReturnIcon = false;

	m_isStartTransition = false;
	m_isCompleteUpper = false;
	m_returnTitlePosEasingTimer = 0;
	m_transitionDelayTimer = 0;

	SlowMgr::Instance()->Init();

	m_isFeverCameraEffect = false;
	m_feverNearCameraTimer = 0;

	ShakeMgr::Instance()->Init();

	DrawFunc_Append::RegisterRenderTargets(D3D12App::Instance()->GetBackBuffFormat(), m_emissiveMap, m_depthMap, m_depthStencil);
}

void GameScene::OnUpdate()
{
	/*===== 更新処理 =====*/

	//現在のカメラ取得
	auto& nowCam = *GameManager::Instance()->GetNowCamera();

	//スクリーンサイズを取得。
	Vec2<float> windowSize = Vec2<float>(WinApp::Instance()->GetWinSize().x, WinApp::Instance()->GetWinSize().y);

	//ゲームマネージャ更新
	GameManager::Instance()->Update();

	// シーン遷移のカメラが上を向ききっっていなかったら。　リザルト用で実装した処理で、ゲーム中は常に通ります。
	if (!m_isCompleteUpper) {

		// プレイヤー更新処理
		m_player->Update(nowCam, m_bulletMgr, m_enemyMgr, windowSize, MAP_SIZE, EDGE_SCOPE);

		// 敵更新処理
		m_enemyMgr->Update(m_bulletMgr, m_player->GetPos(), MAP_SIZE);

		// 弾を更新。
		m_bulletMgr->Update(MAP_SIZE);

		// 敵Waveクラスの更新処理。
		if (EnemyWaveEditor::Instance()->CanWaveUpdate())
		{
			m_enemyWaveMgr->Update(m_enemyMgr, m_player->GetPos(), MAP_SIZE);
		}

	}
	else {

		// 遷移が始まっていて、カメラが上を向ききっていたらプレイヤーを初期化し続ける。
		if (m_isCompleteUpper) {

			m_player->Init();

		}

	}



	if (UsersInput::Instance()->KeyOnTrigger(DIK_SPACE)) {

		GameMode::Instance()->m_id = GameMode::ID::RESULT;

		// 現在フィーバー状態だったらデフォルトに戻す。
		if (m_environmentMgr->GetNowStatus() == EnvironmentMgr::FEVER) {

			m_environmentMgr->ChangeStatus(EnvironmentMgr::DEFAULT);

		}

	}



	// フィーバーのトリガーを取得
	if (!m_isFeverCameraEffect) {
		m_isFeverCameraEffect = m_player->GetIsFeverTrigger();
	}
	if (m_player->GetIsFeverEndTrigger()) {
		// 通常状態にする。
		m_environmentMgr->ChangeStatus(EnvironmentMgr::DEFAULT);
	}

	// フィーバー中のカメラ演出中だったら。
	if (m_isFeverCameraEffect) {

		SlowMgr::Instance()->m_baseSlow = 0.0f;

		// 現在のタイマーの割合を求める。
		float nowRate = static_cast<float>(m_feverNearCameraTimer) / static_cast<float>(FEVER_NEAR_CAMERA_TIMER);

		// 現在のシェイク量。
		float nowShake = nowRate * FEVER_SHAKE;
		Vec3<float> shake = Vec3<float>(KuroFunc::GetRand(-nowShake, nowShake), KuroFunc::GetRand(-nowShake, nowShake), KuroFunc::GetRand(-nowShake, nowShake));

		// カメラをプレイヤーに近づける。	
		const float CAMERA_DISTANCE = 30.0f;
		m_baseEye = m_player->GetPos() + Vec3<float>(CAMERA_DISTANCE, CAMERA_DISTANCE, 0) + shake;
		m_nowEye += (m_baseEye - m_nowEye) / 3.0f;
		m_gameCam->SetPos(m_nowEye);

		// カメラの注視点を補間。
		m_baseTarget = m_player->GetPos();
		m_baseTarget.x += 3.0f;
		m_nowTarget += (m_baseTarget - m_nowTarget) / 3.0f;
		m_gameCam->SetTarget(m_nowTarget);


		// 一定時間経過したらフィーバーのカメラ演出を終える。
		++m_feverNearCameraTimer;
		if (FEVER_NEAR_CAMERA_TIMER < m_feverNearCameraTimer) {

			m_feverNearCameraTimer = 0;
			m_isFeverCameraEffect = false;
			SlowMgr::Instance()->m_baseSlow = 1.0f;

			// フィーバー状態にする。
			m_environmentMgr->ChangeStatus(EnvironmentMgr::FEVER);

		}

	}
	// ゲームの状態に応じてカメラの位置を変える。
	else if (GameMode::Instance()->m_id == GameMode::ID::GAME) {

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
	else {

		UpdateResult();

	}

	// チュートリアル状態の時、エンターキーを押すことでゲームモードのカメラに移行する。
	if (GameMode::Instance()->m_id == GameMode::ID::TUTORIAL && UsersInput::Instance()->KeyInput(DIK_RETURN)) {
		GameMode::Instance()->m_id = GameMode::ID::GAME;
	}
	if (GameMode::Instance()->m_id == GameMode::ID::GAME && UsersInput::Instance()->KeyInput(DIK_SPACE)) {
		GameMode::Instance()->m_id = GameMode::ID::TUTORIAL;
	}

	if (m_player->GetIsFever())
	{
		m_environmentMgr->ChangeColor(m_enemyMgr->GetParticleColor());
	}
	else
	{
		m_environmentMgr->ChangeColor(Color(0.0f, 0.0f, 0.0f, 1.0f));
	}

	if (m_enemyWaveMgr->IsNowWaveBounusStage() || UsersInput::Instance()->KeyInput(DIK_G))
	{
		m_bonusEffect->Start();
	}
	else if(m_bonusEffect->IsStart())
	{
		m_bonusEffect->Stop();
	}


	m_environmentMgr->Update(m_player->GetPos());
	m_feverGauge->Update(m_player->GetIsFever(), m_player->GetPlayerFeverRate());

	m_gameUI->Update();


	SlowMgr::Instance()->Update();

	m_bonusEffect->Update();

	ShakeMgr::Instance()->Update();

	// カメラを動かす。
	m_gameCam->SetPos(m_gameCam->GetPos() + ShakeMgr::Instance()->m_shake);

}

#include "DrawFunc2D.h"
void GameScene::OnDraw()
{
	/*===== 描画処理 =====*/

	// ステージのレンダーターゲットをセット。




	//バックバッファ取得
	auto backBuff = D3D12App::Instance()->GetBackBuffRenderTarget();


	GameManager::Instance()->ChangeCamera(m_gameCamKey);
	//現在のカメラ取得
	auto& nowCam = GameManager::Instance()->GetNowCamera();


	//DrawFunc初期化
	DrawFunc_Append::FrameInit(
		backBuff,
		nowCam,
		m_environmentMgr->GetLigMgr()
	);

	/*--- 通常描画 ---*/

	//環境描画
	m_environmentMgr->Draw(*nowCam);

	// マップを描画
	StageFloor::Instance()->ScreenTargetSet(true);
	m_gameUI->Draw();
	m_bonusEffect->Draw();
	StageFloor::Instance()->Draw();

	//プレイヤー描画
	m_player->Draw(*nowCam);

	//敵を描画
	m_enemyMgr->Draw(*nowCam, backBuff, m_emissiveMap, m_depthStencil);

	//弾を描画。
	m_bulletMgr->Draw();

	// フィーバーゲージを描画。
	m_feverGauge->Draw();

	// 戻るのアイコンを描画。
	DrawFunc2D::DrawExtendGraph2D(RETURN_ICON_POS - m_returnIconSize, RETURN_ICON_POS + m_returnIconSize, m_returnTexture);


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
	ImGui::Begin("Player");
	ImGui::Text("POS_X:%f", m_player->GetPos().x);
	ImGui::Text("POS_Y:%f", m_player->GetPos().y);
	ImGui::Text("POS_Z:%f", m_player->GetPos().z);
	ImGui::End();

	//ウェーブ編集
	EnemyWaveEditor::Instance()->EditWithImgui(*m_enemyWaveMgr, m_enemyMgr);

	m_gameUI->DrawImGui();
}

void GameScene::OnFinalize()
{
}

void GameScene::UpdateResult()
{


	// マウスカーソルと戻るのアイコンの当たり判定を行う。
	Vec2<float> mousePos = UsersInput::Instance()->GetMousePos();
	float length = Vec2<float>(mousePos - RETURN_ICON_POS).Length();
	if (length < RETURN_ICON_SIZE.x * 2.0f) {

		m_isNearReturnIcon = true;

	}
	else {

		m_isNearReturnIcon = false;

	}

	// 戻るのアイコンのサイズを調整。
	if (m_isNearReturnIcon) {
		m_returnIconSize.x += (RETURN_ICON_EXP_SIZE.x - m_returnIconSize.x) / 2.0f;
		m_returnIconSize.y += (RETURN_ICON_EXP_SIZE.y - m_returnIconSize.y) / 2.0f;

		// クリックされていたら遷移させる。
		if (UsersInput::Instance()->MouseOnTrigger(LEFT)) {

			m_isStartTransition = true;

		}

	}
	else {
		m_returnIconSize.x += (RETURN_ICON_SIZE.x - m_returnIconSize.x) / 2.0f;
		m_returnIconSize.y += (RETURN_ICON_SIZE.y - m_returnIconSize.y) / 2.0f;
	}

	// 遷移が始まっていたら。

	//現在のカメラ取得
	auto& nowCam = GameManager::Instance()->GetNowCamera();

	if (m_isStartTransition) {

		// 上を向ききっていたら。
		if (m_isCompleteUpper) {

			// カメラをタイトルのカメラの位置まで持っていくイージングのタイマーを更新。
			m_returnTitlePosEasingTimer += 0.03f;
			if (1.0f <= m_returnTitlePosEasingTimer) m_returnTitlePosEasingTimer = 1.0f;
			float easingAmount = KuroMath::Ease(InOut, Exp, m_returnTitlePosEasingTimer, 0.0f, 1.0f);

			// カメラの位置をセット。
			nowCam->SetTarget(m_baseEasingCameraTarget + (TITLE_TARGET_POS - m_baseEasingCameraTarget) * easingAmount);
			nowCam->SetPos(m_baseEasingCameraEye + (TITLE_EYE_POS - m_baseEasingCameraEye) * easingAmount);


			// 補間先との距離が一定以下になったら遷移させる。
			if (1.0f <= m_returnTitlePosEasingTimer) {

				++m_transitionDelayTimer;
				if (30 < m_transitionDelayTimer) {
					KuroEngine::Instance()->ChangeScene(0, m_sceneTransition.get());

				}
			}

		}
		else {

			Vec3<float> nowCameraTarget = nowCam->GetTarget();

			nowCameraTarget.y += (TRANSITION_CAMERA_Y - nowCameraTarget.y) / 5.0f;

			nowCam->SetTarget(nowCameraTarget);

			// 補間先までの距離が一定以下になったら次へ
			if (fabs(nowCameraTarget.y - TRANSITION_CAMERA_Y) <= 1.0f) {

				m_isCompleteUpper = true;

				m_baseEasingCameraEye = nowCam->GetPos();
				m_baseEasingCameraTarget = nowCam->GetTarget();

			}

		}

	}
	else {


		// カメラを引く。
		m_nowEye += (CAMERA_HOME_EYE_POSITION - m_nowEye) / 5.0f;
		m_nowTarget += (CAMERA_HOME_TARGET_POSITION - m_nowTarget) / 5.0f;
		m_gameCam->SetPos(m_nowEye);
		m_gameCam->SetTarget(m_nowTarget);

	}

}
