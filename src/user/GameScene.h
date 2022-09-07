#pragma once
#include"KuroEngine.h"
#include"Transform.h"
#include"DebugCamera.h"
#include"Player.h"
#include"Vec.h"
#include<memory>

#include"LightBloomDevice.h"
class DepthStencil;
class Player;
class Model;
class ModelObject;
class Camera;
class RenderTarget;
class EnemyMgr;
class BulletMgr;

class GameScene : public BaseScene
{
private:
	//�v���C���[
	std::unique_ptr<Player> m_player;

	//�}�b�v���f��
	std::shared_ptr<ModelObject>m_mapModel;

	//�G�N���X�B
	std::shared_ptr<EnemyMgr> m_enemyMgr;

	//�Q�[���J����
	const std::string m_gameCamKey = "GameCamera";
	std::shared_ptr<Camera>m_gameCam;

	// �N���X
	std::shared_ptr<BulletMgr> m_bulletMgr;

	//�}�b�v�̃T�C�Y
	const float MAP_SIZE = 100.0f;
	const float EDGE_SCOPE = 10.0f;

	//���C�g�u���[��
	LightBloomDevice m_ligBloomDev;

	//�f�v�X�X�e���V��
	std::shared_ptr<DepthStencil>m_depthStencil;

	//�G�~�b�V�u�}�b�v
	std::shared_ptr<RenderTarget>m_emissiveMap;

	bool m_emissive = false;

public:
	GameScene();
	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;
};