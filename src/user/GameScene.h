#pragma once
#include"KuroEngine.h"
#include"Transform.h"
#include"DebugCamera.h"
#include"Player.h"
#include"Vec.h"
#include<memory>
#include"GameTimer.h"

#include"LightBloomDevice.h"
class DepthStencil;
class Player;
class Model;
class ModelObject;
class Camera;
class RenderTarget;
class EnemyMgr;
class BulletMgr;
class EnvironmentMgr;
class EnemyWaveMgr;
class GameTimer;

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

	// �G�E�F�[�u�N���X
	std::unique_ptr<EnemyWaveMgr> m_enemyWaveMgr;

	// �t�B�[�o�[�̃^�C�}�[�pUI
	std::unique_ptr<GameTimer> m_feverGameTimer;

	//�}�b�v�̃T�C�Y
	float MAP_SIZE = 150.0f;
	const float EDGE_SCOPE = 10.0f;

	//���C�g�u���[��
	LightBloomDevice m_ligBloomDev;

	//�J�����֌W�B
	Vec3<float> m_baseEye;
	Vec3<float> m_nowEye;
	Vec3<float> m_baseTarget;
	Vec3<float> m_nowTarget;

	//�f�v�X�X�e���V��
	std::shared_ptr<DepthStencil>m_depthStencil;

	//�G�~�b�V�u�}�b�v
	std::shared_ptr<RenderTarget>m_emissiveMap;

	//�f�v�X�}�b�v
	std::shared_ptr<RenderTarget>m_depthMap;

	//������
	std::unique_ptr<EnvironmentMgr>m_environmentMgr;

	std::unique_ptr<GameTimer>m_gameTimer;

	bool m_emissive = true;


public:
	GameScene();
	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;
};