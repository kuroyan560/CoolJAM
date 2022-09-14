#pragma once
#include"KuroEngine.h"
#include"Transform.h"
#include"DebugCamera.h"
#include"Player.h"
#include"Vec.h"
#include<memory>
#include"GameTimer.h"
#include"GameUI.h"
#include"LightBloomDevice.h"
#include"BonusEffect.h"
#include"EnemyWaveMgr.h"
#include"LightBloomDevice.h"

class DepthStencil;
class Player;
class Model;
class ModelObject;
class Camera;
class RenderTarget;
class EnemyMgr;
class BulletMgr;
class EnvironmentMgr;
class EnemyWaveMgr;
class GameTimer;
class TextureBuffer;
class FeverGauge;
class SceneTransition;

class GameScene : public BaseScene
{
private:
	//プレイヤー
	std::unique_ptr<Player> m_player;

	//敵クラス。
	std::shared_ptr<EnemyMgr> m_enemyMgr;

	//ゲームカメラ
	const std::string m_gameCamKey = "GameCamera";
	std::shared_ptr<Camera>m_gameCam;

	// クラス
	std::shared_ptr<BulletMgr> m_bulletMgr;

	// 敵ウェーブクラス
	std::unique_ptr<EnemyWaveMgr> m_enemyWaveMgr;

	// フィーバーゲージ
	std::unique_ptr<FeverGauge> m_feverGauge;

	//マップのサイズ
	float MAP_SIZE = 150.0f;
	const float EDGE_SCOPE = 10.0f;

	// チュートリアル、リザルトでのカメラの位置。
	Vec3<float> CAMERA_HOME_TARGET_POSITION = Vec3<float>(MAP_SIZE / 5.0f, 0.0f, 0.0f);
	Vec3<float> CAMERA_HOME_EYE_POSITION = Vec3<float>(MAP_SIZE, MAP_SIZE * 2.0f, 0.0f);

	//ライトブルーム
	LightBloomDevice m_ligBloomDev;

	//カメラ関係。
	Vec3<float> m_baseEye;
	Vec3<float> m_nowEye;
	Vec3<float> m_baseTarget;
	Vec3<float> m_nowTarget;

	//デプスステンシル
	std::shared_ptr<DepthStencil>m_depthStencil;

	//エミッシブマップ
	std::shared_ptr<RenderTarget>m_emissiveMap;

	//デプスマップ
	std::shared_ptr<RenderTarget>m_depthMap;

	//環境統括
	std::unique_ptr<EnvironmentMgr>m_environmentMgr;

	std::unique_ptr<GameTimer>m_gameTimer;

	std::unique_ptr<GameUI>m_gameUI;

	std::unique_ptr<BonusEffect>m_bonusEffect;

	int m_bgm;

	bool m_emissive = true;

	bool m_isStartTransition;
	bool m_isCompleteUpper;
	Vec3<float> m_baseEasingCameraEye;
	Vec3<float> m_baseEasingCameraTarget;
	float m_returnTitlePosEasingTimer;
	int m_transitionDelayTimer;

	// 遷移するときに見るカメラのY軸座標
	const float TRANSITION_CAMERA_Y = 400.0f;

	//シーン遷移
	std::unique_ptr<SceneTransition> m_sceneTransition;

	// タイトルでの各カメラのパラメーター
	Vec3<float> TITLE_TARGET_POS = Vec3<float>(3.19659996f, 0.100950003f, -4.14720011f);
	Vec3<float> TITLE_EYE_POS = Vec3<float>(9.55155659f, 2.39459991f, 5.30036783f);

	// 戻るのアイコン。
	std::shared_ptr<TextureBuffer> m_returnTexture;
	bool m_isNearReturnIcon;
	Vec2<float> m_returnIconSize;
	Vec2<float> RETURN_ICON_SIZE = Vec2<float>(64.0f, 64.0f);
	Vec2<float> RETURN_ICON_EXP_SIZE = Vec2<float>(94.0f, 94.0f);
	Vec2<float> RETURN_ICON_POS = Vec2<float>(RETURN_ICON_SIZE.x * 2.0f, WinApp::Instance()->GetExpandWinSize().y - RETURN_ICON_SIZE.y * 2.0f);



public:
	GameScene();
	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;


private:

	void UpdateResult();

};