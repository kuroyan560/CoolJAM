#pragma once
#include "D3D12App.h"
#include <memory>

class TutorialText {

private:

	/*===== �����o�ϐ� =====*/

	std::shared_ptr<RenderTarget> m_floorRenderTarget;


public:

	// �`���[�g���A���̃X�e�[�^�X
	enum class TUTORIAL_STATUS {

		MOUSE,			// �}�E�X�̕����ɂ����`���[�g���A��
		DASH,			// �_�b�V���̃`���[�g���A���B
		DAMAGE_WALL,	// �_���[�W�ǂ̐���
		ENEMY,			// �^�񒆂ɉ������Ă��Ȃ��G��z�u
		FEVER,			// �t�B�[�o�[��Ԃ̐���

	};

	TUTORIAL_STATUS m_tutorialStatus;


public:

	/*===== �����o�֐� =====*/

	TutorialText();
	void Init();
	void Update();
	void Draw();

	void Next();

	TUTORIAL_STATUS GetNowStatus() { return m_tutorialStatus; }

};