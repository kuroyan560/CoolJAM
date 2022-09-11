#pragma once
#include"Vec.h"
#include"KuroEngine.h"
#include<memory>
#include"LightBloomDevice.h"

class Player;
class Camera;
class DepthStencil;
class EnvironmentMgr;
class RenderTarget;

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
	Vec3<float> DEF_TARGET_POS = Vec3<float>(3.19660044f, 0.100951008f, -4.14727974f);
	Vec3<float> DEF_EYE_POS = Vec3<float>(9.01069641f, 2.39467144f, 6.33004093f);
	const float DEF_ANGLE = 0.506614208f;
	const float DEF_LENGTH = 10.9236498f;

	// �J�����̕�Ԍ�̈ʒu
	Vec3<float> END_TARGET_POS = Vec3<float>(0.0f, 0.0f, 0.0f);
	Vec3<float> END_EYE_POS = Vec3<float>(80.0f, 80.0f, 0.0f);
	const float END_ANGLE = 0;
	const float CHANGE_ANGLE = DEF_ANGLE - DEF_ANGLE + END_ANGLE;	// �p�x�̕ۊǗ�
	const float END_LENGTH = 80.0f;


public:

	TitleScene();
	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;

};