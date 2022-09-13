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
	// �����_�[�^�[�Q�b�g���Z�b�g�B
	StageFloor::Instance()->ScreenTargetSet(true);

	timer->Draw();

}