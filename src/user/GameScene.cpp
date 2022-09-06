#include "GameScene.h"
#include "Player.h"

GameScene::GameScene()
{

	m_player = std::make_unique<Player>();
	m_player->Init();

}

void GameScene::OnInitialize()
{

	m_player->Init();

}

void GameScene::OnUpdate()
{

	m_player->Update();

}

void GameScene::OnDraw()
{

	m_player->Draw();

}

void GameScene::OnImguiDebug()
{
}

void GameScene::OnFinalize()
{
}