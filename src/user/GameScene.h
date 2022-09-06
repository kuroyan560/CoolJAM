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

	//�v���C���[
	std::unique_ptr<Player> m_player;

	//�f�v�X�X�e���V��
	std::shared_ptr<DepthStencil>m_depthStencil;

public:
	GameScene();
	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;
};