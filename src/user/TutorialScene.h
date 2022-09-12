#pragma once
#include"KuroEngine.h"
#include"Transform.h"
#include"DebugCamera.h"
#include"Player.h"
#include"Vec.h"
#include<memory>
#include"GameTimer.h"
#include"GrazeEmitter.h"

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
class TextureBuffer;
class FeverGauge;
class TutorialText;

class TutorialScene : public BaseScene
{
private:
	//�v���C���[
	std::unique_ptr<Player> m_player;

	//�}�b�v���f��
	std::shared_ptr<ModelObject>m_mapModel;

	//�G�N���X�B
	std::shared_ptr<EnemyMgr> m_enemyMgr;

	//�Q�[���J����
	const std::string m_gameCamKey = "TutorialCamera";
	std::shared_ptr<Camera>m_gameCam;

	// �N���X
	std::shared_ptr<BulletMgr> m_bulletMgr;

	// �G�E�F�[�u�N���X
	std::unique_ptr<EnemyWaveMgr> m_enemyWaveMgr;

	// �t�B�[�o�[�Q�[�W
	std::unique_ptr<FeverGauge> m_feverGauge;

	//�}�b�v�̃T�C�Y
	float MAP_SIZE = 150.0f;
	const float EDGE_SCOPE = 10.0f;

	// �`���[�g���A���A���U���g�ł̃J�����̈ʒu�B
	Vec3<float> CAMERA_HOME_TARGET_POSITION = Vec3<float>(MAP_SIZE / 5.0f, 0.0f, 0.0f);
	Vec3<float> CAMERA_HOME_EYE_POSITION = Vec3<float>(MAP_SIZE, MAP_SIZE * 2.0f, 0.0f);
	bool m_isCameraHomePosition;

	//���C�g�u���[��
	LightBloomDevice m_ligBloomDev;

	// �`���[�g���A���̃e�L�X�g
	std::shared_ptr<TutorialText> m_tutorialText;


	//�J�����֌W�B
	Vec3<float> m_baseEye;
	Vec3<float> m_nowEye;
	Vec3<float> m_baseTarget;
	Vec3<float> m_nowTarget;

	std::unique_ptr<GrazeEmitter> m_grazeEmitter;

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


	/*-- �`���[�g���A���Ɏg�p����ϐ� --*/

	int m_damageWallTimer;		// �_���[�W�ǂ̃`���[�g���A���̕\�����ԁB
	const int DAMAGE_WALL_TUTORIAL_TIMER = 240;

	int m_enemyTutorialAppearTimer;		// �G�̃`���[�g���A���œG�̐����̃e�L�X�g��\�����鎞�ԁB
	const int ENEMY_TUTORIAL_APPEAR_TIMER = 180;
	int m_enemyTutorialExitTimer;		// �G�̃`���[�g���A���œG��|���Ă��玟�̃`���[�g���A���ɑJ�ڂ���܂ł̎��ԁB
	const int ENEMY_TUTORIAL_EXIT_TIMER = 60;

	int m_feverTutorialAppearTimer;		// �t�B�[�o�[�̃`���[�g���A���ŃG���L�����o������܂ł̎��ԁB
	const int FEVER_TUTORIAL_APPEAR_TIMER = 180;

	int m_feverTutorialSpawnDelay;		// �t�B�[�o�[���ɓG�𕦂�����x��
	const int FEVER_TUTORIAL_SPAWN_DELAY = 60;
	bool m_isFever;						// �t�B�[�o�[�ɂȂ������ǂ���


public:
	TutorialScene();
	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;
};