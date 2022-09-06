#pragma once
#include"KuroEngine.h"
#include<memory>

class Player;

class GameScene : public BaseScene
{
private:
	std::unique_ptr<Player> m_player;

public:
	GameScene();
	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;
};