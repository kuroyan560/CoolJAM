#include "TutorialText.h"

TutorialText::TutorialText() {

	/*===== �R���X�g���N�^ =====*/

	m_tutorialStatus = TUTORIAL_STATUS::MOUSE;
	m_floorRenderTarget = D3D12App::Instance()->GenerateRenderTarget(DXGI_FORMAT_R8G8B8A8_UNORM, Color(0, 0, 0, 0), Vec2<int>(512, 512), L"FloorScreen");

}

void TutorialText::Init() {

	/*===== ���������� =====*/

	m_tutorialStatus = TUTORIAL_STATUS::MOUSE;

}

void TutorialText::Update() {

	/*===== �X�V���� =====*/

	switch (m_tutorialStatus)
	{
	case TutorialText::TUTORIAL_STATUS::MOUSE:

		/*-- �}�E�X��UI�̃e�N�X�`�����X�N���[�� --*/

		break;
	case TutorialText::TUTORIAL_STATUS::DAMAGE_WALL:

		/*-- �_���[�W�ǂ̐�����UI�̃e�N�X�`�����X�N���[�� --*/

		break;
	case TutorialText::TUTORIAL_STATUS::ENEMY:

		/*-- �G�̐�����UI�̃e�N�X�`�����X�N���[�� --*/

		break;
	case TutorialText::TUTORIAL_STATUS::FEVER:

		/*-- �t�B�[�o�[�̐�����UI�̃e�N�X�`�����X�N���[�� --*/

		break;
	default:
		break;
	}

}

void TutorialText::Draw() {

	/*===== �`�揈�� =====*/

}

void TutorialText::Next() {

	/*===== ���� =====*/

	// �t�B�[�o�[����Ȃ�������B(�t�B�[�o�[�͍Ō�̃`���[�g���A���Ȃ̂Ŏ��͂Ȃ��B)
	if (m_tutorialStatus != TUTORIAL_STATUS::FEVER) {

		m_tutorialStatus = static_cast<TUTORIAL_STATUS>(static_cast<int>(m_tutorialStatus) + 1);

	}

}