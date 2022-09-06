#pragma once
#include"KuroEngine.h"
#include"DebugCamera.h"
#include"Player.h"
class DepthStencil;

class GameScene : public BaseScene
{
	//�v���C���[
	Player m_player;

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