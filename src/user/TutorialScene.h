#pragma once
#include"KuroEngine.h"
#include"Transform.h"
#include"DebugCamera.h"
#include"Player.h"
#include"Vec.h"
#include<memory>
#include"GameTimer.h"
#include"GrazeEmitter.h"

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
class Tutorial;

class TutorialScene : public BaseScene
{
private:
	//プレイヤー
	std::shared_ptr<Player> m_player;

	//マップモデル
	std::shared_ptr<ModelObject>m_mapModel;

	//敵クラス。
	std::shared_ptr<EnemyMgr> m_enemyMgr;

	//ゲームカメラ
	const std::string m_gameCamKey = "TutorialCamera";
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
	bool m_isCameraHomePosition;

	//ライトブルーム
	LightBloomDevice m_ligBloomDev;

	// チュートリアル
	std::shared_ptr<Tutorial> m_tutorial;


	//カメラ関係。
	Vec3<float> m_baseEye;
	Vec3<float> m_nowEye;
	Vec3<float> m_baseTarget;
	Vec3<float> m_nowTarget;

	std::unique_ptr<GrazeEmitter> m_grazeEmitter;

	//デプスステンシル
	std::shared_ptr<DepthStencil>m_depthStencil;

	//エミッシブマップ
	std::shared_ptr<RenderTarget>m_emissiveMap;

	//デプスマップ
	std::shared_ptr<RenderTarget>m_depthMap;

	//環境統括
	std::unique_ptr<EnvironmentMgr>m_environmentMgr;

	std::unique_ptr<GameTimer>m_gameTimer;

	// 遷移関係
	bool m_isTransitionStart;
	bool m_isInitPlayer;
	bool m_isCompleteUpper;

	// 遷移するときに見るカメラのY軸座標
	const float TRANSITION_CAMERA_Y = 300.0f;

	// タイトルでの各カメラのパラメーター
	Vec3<float> TITLE_TARGET_POS = Vec3<float>(3.19660044f, 0.100951008f, -4.14727974f);
	Vec3<float> TITLE_EYE_POS = Vec3<float>(9.01069641f, 2.39467144f, 6.33004093f);

	bool m_emissive = true;


public:
	TutorialScene();
	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;
};