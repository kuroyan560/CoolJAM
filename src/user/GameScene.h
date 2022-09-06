#pragma once
#include"KuroEngine.h"
#include"DebugCamera.h"
#include"Player.h"
#include"LightBloomDevice.h"
class DepthStencil;
class RenderTarget;

class GameScene : public BaseScene
{
	//プレイヤー
	Player m_player;

	//ライトブルーム
	LightBloomDevice m_ligBloomDev;

	//メインレンダーターゲット
	std::shared_ptr<RenderTarget>m_mainTarget;

	//デプスステンシル
	std::shared_ptr<DepthStencil>m_depthStencil;

	//エミッシブマップ
	std::shared_ptr<RenderTarget>m_emissiveMap;

public:
	GameScene();
	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;
};