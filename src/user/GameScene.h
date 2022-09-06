#pragma once
#include"KuroEngine.h"
#include"DebugCamera.h"
#include"Player.h"
#include"LightBloomDevice.h"
class DepthStencil;
class RenderTarget;

class GameScene : public BaseScene
{
	//�v���C���[
	Player m_player;

	//���C�g�u���[��
	LightBloomDevice m_ligBloomDev;

	//���C�������_�[�^�[�Q�b�g
	std::shared_ptr<RenderTarget>m_mainTarget;

	//�f�v�X�X�e���V��
	std::shared_ptr<DepthStencil>m_depthStencil;

	//�G�~�b�V�u�}�b�v
	std::shared_ptr<RenderTarget>m_emissiveMap;

public:
	GameScene();
	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;
};