#pragma once
#include"KuroEngine.h"
#include"Transform.h"
#include"DebugCamera.h"
#include"Player.h"
#include"Vec.h"
#include<memory>
#include"GrazeEmitter.h"

class DepthStencil;
class Player;
class Model;

class GameScene : public BaseScene
{
private:

	//�v���C���[
	std::unique_ptr<Player> m_player;

	//�}�b�v���f��
	std::shared_ptr<Model> m_mapModel;
	Transform m_mapModelTransform;
	Vec3<float>limitPos;

	//�}�b�v�̃T�C�Y
	const float MAP_SIZE = 100.0f;
	const float EDGE_SCOPE = 10.0f;

	std::unique_ptr<GrazeEmitter> m_grazeEmitter;

	//�f�v�X�X�e���V��
	std::shared_ptr<DepthStencil>m_depthStencil;

	std::shared_ptr<TextureBuffer>texBuff;


public:
	GameScene();
	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;
};