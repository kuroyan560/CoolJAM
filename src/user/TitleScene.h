#pragma once
#include"Vec.h"
#include"KuroEngine.h"
#include"Transform.h"
#include<memory>
#include"LightBloomDevice.h"

class Player;
class Camera;
class DepthStencil;
class Model;
class EnvironmentMgr;
class RenderTarget;
class TitleUI;
class BulletMgr;
class EnemyMgr;
class EnemyWaveMgr;

class TitleScene : public BaseScene {

private:

	std::unique_ptr<Player> m_player;	// �v���C���[

	// �^�C�g���J����
	const std::string m_gameCamKey = "TitleCamera";
	std::shared_ptr<Camera>m_gameCam;

	//�J�����֌W�B
	Vec3<float> m_baseEye;
	Vec3<float> m_nowEye;
	Vec3<float> m_baseTarget;
	Vec3<float> m_nowTarget;

	// �N���X
	std::shared_ptr<BulletMgr> m_bulletMgr;

	// �G�E�F�[�u�N���X
	std::unique_ptr<EnemyWaveMgr> m_enemyWaveMgr;

	//�G�N���X�B
	std::shared_ptr<EnemyMgr> m_enemyMgr;

	// �}�b�v���f��
	float MAP_SIZE = 150.0f;
	std::shared_ptr<ModelObject> m_mapModel;
	Transform m_mapModelTransform;

	// �^�C�g���摜
	std::shared_ptr<TextureBuffer> m_titleTexture;
	float m_titleAngle;
	bool m_isTitle;

	// UI
	enum class SELECT {
		MIN,
		TUTORIAL,
		GAME,
		EXIT,
		MAX,
	};
	SELECT m_nowSelect;
	std::array<std::shared_ptr<TextureBuffer>, 3> m_selectUI;

	// UI�p�ϐ�
	Vec2<float> m_revolverPos;
	const Vec2<float> REVOLVER_POS = Vec2<float>(-100, 720.0f / 2.0f);
	const Vec2<float> OFF_SCREEN_REVOLVER_POS = Vec2<float>(-900.0f, 720.0f / 2.0f);
	float m_revolverEasingTimer;
	const float ADD_REVOLVER_EASING_TIMER = 0.03f;



	std::array<std::shared_ptr<TitleUI>, 3> m_titleUI;


	// �o���ޏo�t���O
	bool m_isAppear;

	//�f�v�X�X�e���V��
	std::shared_ptr<DepthStencil>m_depthStencil;

	//�G�~�b�V�u�}�b�v
	std::shared_ptr<RenderTarget>m_emissiveMap;

	//�f�v�X�}�b�v
	std::shared_ptr<RenderTarget>m_depthMap;

	//������
	std::unique_ptr<EnvironmentMgr>m_environmentMgr;

	//�V�[���J��
	std::unique_ptr<SceneTransition> m_sceneTransition;

	//���C�g�u���[��
	LightBloomDevice m_ligBloomDev;

	bool m_emissive = true;

	bool m_isTransition;

	// �C�[�W���O���I����Ă���Q�[���V�[���ɑJ�ڂ���܂ł̃^�C�}�[
	int m_endEasingTransitionTimer;
	const int END_EASING_TIMER = 90;

	// �J�ڎ��̃C�[�W���O�^�C�}�[
	float m_transitionEasingTimer;
	const float ADD_TRANSITION_EASING_TIMER = 0.01f;

	// �J�����̏����ʒu
	Vec3<float> DEF_TARGET_POS = Vec3<float>(3.1966f, 0.10095f, -4.1472f);
	Vec3<float> DEF_EYE_POS = Vec3<float>(9.0106f, 2.3946f, 6.3300f);
	const float DEF_ANGLE = 0.506614208f;
	const float DEF_LENGTH = 10.9236498f;


	// �J�����̕�Ԍ�̈ʒu
	Vec3<float> END_GAME_TARGET_POS = Vec3<float>(0.0f, 0.0f, 0.0f);
	Vec3<float> END_GAME_EYE_POS = Vec3<float>(80.0f, 80.0f, 0.0f);
	Vec3<float> END_TUTORIAL_TARGET_POS = Vec3<float>(150.0f / 5.0f, 0.0f, 0.0f);
	Vec3<float> END_TUTORIAL_EYE_POS = Vec3<float>(150.0f, 300.0f, 0.0f);
	const float END_ANGLE = 0;
	const float CHANGE_ANGLE = DEF_ANGLE - DEF_ANGLE + END_ANGLE;	// �p�x�̕ۊǗ�
	const float END_GAME_LENGTH = 80.0f;
	const float END_TUTORIAL_LENGTH = 150.0f;

	//SE
	int m_selectSE;
	int m_enterSE;

public:
	TitleScene();
	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;

private:

	void UpdateSelect();
	void UpdateCamera();

};