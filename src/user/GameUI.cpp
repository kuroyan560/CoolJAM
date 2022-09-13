#include"GameUI.h"

GameUI::GameUI()
{
	timer = std::make_unique<GameTimer>();
}

void GameUI::Init()
{
	timer->Init(120);
}

void GameUI::Update()
{
	timer->Update();
}

void GameUI::Draw()
{
	// レンダーターゲットをセット。
	StageFloor::Instance()->ScreenTargetSet(true);

	timer->Draw();

}