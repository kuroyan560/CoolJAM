#include "Player.h"
#include"Importer.h"

Player::Player()
{
	m_model = Importer::Instance()->LoadModel("resource/user/", "player.glb");
}

void Player::Init()
{
}

void Player::Update()
{
}

#include"DrawFunc3D.h"
void Player::Draw(Camera& Cam)
{
	DrawFunc3D::DrawNonShadingModel(m_model, m_transform, Cam);
}

void Player::Finalize()
{
}
