#include"GameUI.h"

GameUI::GameUI()
{
	timer = std::make_unique<GameTimer>();
}

void GameUI::Init()
{
	timer->Init(120);
	timer->Start();
}

void GameUI::Update()
{
	timer->Update();
}

void GameUI::Draw()
{
	timer->Draw();
}