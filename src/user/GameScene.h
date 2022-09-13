#pragma once
#include"KuroEngine.h"
#include"Transform.h"
#include"DebugCamera.h"
#include"Player.h"
#include"Vec.h"
#include<memory>
#include"GameTimer.h"
#include"GameUI.h"

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
class SceneTransition;

class GameScene : public BaseScene
{
private:
	//�v���C���[
	std::unique_ptr<Player> m_player;

	//�G�N���X�B
	std::shared_ptr<EnemyMgr> m_enemyMgr;

	//�Q�[���J����
	const std::string m_gameCamKey = "GameCamera";
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

	std::unique_ptr<GameUI>m_gameUI;

	int m_bgm;

	bool m_emissive = true;

	bool m_isStartTransition;
	bool m_isCompleteUpper;
	Vec3<float> m_baseEasingCameraEye;
	Vec3<float> m_baseEasingCameraTarget;
	float m_returnTitlePosEasingTimer;
	int m_transitionDelayTimer;

	// �J�ڂ���Ƃ��Ɍ���J������Y�����W
	const float TRANSITION_CAMERA_Y = 400.0f;

	//�V�[���J��
	std::unique_ptr<SceneTransition> m_sceneTransition;

	// �^�C�g���ł̊e�J�����̃p�����[�^�[
	Vec3<float> TITLE_TARGET_POS = Vec3<float>(3.19659996f, 0.100950003f, -4.14720011f);
	Vec3<float> TITLE_EYE_POS = Vec3<float>(9.55155659f, 2.39459991f, 5.30036783f);

	// �߂�̃A�C�R���B
	std::shared_ptr<TextureBuffer> m_returnTexture;
	bool m_isNearReturnIcon;
	Vec2<float> m_returnIconSize;
	Vec2<float> RETURN_ICON_SIZE = Vec2<float>(64.0f, 64.0f);
	Vec2<float> RETURN_ICON_EXP_SIZE = Vec2<float>(94.0f, 94.0f);
	Vec2<float> RETURN_ICON_POS = Vec2<float>(RETURN_ICON_SIZE.x * 2.0f, WinApp::Instance()->GetExpandWinSize().y - RETURN_ICON_SIZE.y * 2.0f);



public:
	GameScene();
	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;


private:

	void UpdateResult();

};