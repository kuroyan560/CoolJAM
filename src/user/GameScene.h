#pragma once
#include"KuroEngine.h"
#include"Transform.h"
#include"DebugCamera.h"
#include"Player.h"
#include"Vec.h"
#include<memory>

#include"LightBloomDevice.h"
class DepthStencil;
class Player;
class Model;
class ModelObject;
class Camera;
class RenderTarget;

class GameScene : public BaseScene
{
private:

	//プレイヤー
	std::unique_ptr<Player> m_player;

	//マップモデル
	std::shared_ptr<ModelObject>m_mapModel;

	//ゲームカメラ
	const std::string m_gameCamKey = "GameCamera";
	std::shared_ptr<Camera>m_gameCam;

	//マップのサイズ
	const float MAP_SIZE = 100.0f;
	const float EDGE_SCOPE = 10.0f;

	//ライトブルーム
	LightBloomDevice m_ligBloomDev;

	//メインレンダーターゲット
	std::shared_ptr<RenderTarget>m_mainTarget;

	//デプスステンシル
	std::shared_ptr<DepthStencil>m_depthStencil;

	//エミッシブマップ
	std::shared_ptr<RenderTarget>m_emissiveMap;

	bool m_emissive = true;

public:
	GameScene();
	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;
};