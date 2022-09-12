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

class GameScene : public BaseScene
{
private:
	//プレイヤー
	std::unique_ptr<Player> m_player;

	//マップモデル
	std::shared_ptr<ModelObject>m_mapModel;

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

	int m_bgm;

	bool m_emissive = false;



public:
	GameScene();
	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;
};