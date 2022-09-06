#pragma once
#include"KuroEngine.h"
#include"DebugCamera.h"
#include"Player.h"
class DepthStencil;
#include<memory>

class Player;

class GameScene : public BaseScene
{
private:
	std::unique_ptr<Player> m_player;

	//プレイヤー
	Player m_player;

	//デプスステンシル
	std::shared_ptr<DepthStencil>m_depthStencil;

public:
	GameScene();
	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;
};