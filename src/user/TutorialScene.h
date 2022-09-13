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
class Tutorial;

class TutorialScene : public BaseScene
{
private:
	//�v���C���[
	std::shared_ptr<Player> m_player;

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

	// �`���[�g���A��
	std::shared_ptr<Tutorial> m_tutorial;


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

	//�V�[���J��
	std::unique_ptr<SceneTransition> m_sceneTransition;

	// �J�ڊ֌W
	bool m_isTransitionStart;
	bool m_isInitPlayer;
	bool m_isCompleteUpper;
	Vec3<float> m_baseEasingCameraEye;
	Vec3<float> m_baseEasingCameraTarget;
	float m_returnTitlePosEasingTimer;
	int m_transitionDelayTimer;

	// �J�ڂ���Ƃ��Ɍ���J������Y�����W
	const float TRANSITION_CAMERA_Y = 300.0f;

	// �^�C�g���ł̊e�J�����̃p�����[�^�[
	Vec3<float> TITLE_TARGET_POS = Vec3<float>(3.19659996f, 0.100950003f, -4.14720011f);
	Vec3<float> TITLE_EYE_POS = Vec3<float>(9.55155659f, 2.39459991f, 5.30036783f);

	bool m_emissive = true;


public:
	TutorialScene();
	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;
};