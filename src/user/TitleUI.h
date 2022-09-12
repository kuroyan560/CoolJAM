#pragma once
#include "Vec.h"
#include "D3D12Data.h"
#include <memory>
#include <array>
#include <string>

class TitleUI {

private:

	/*===== �����o�ϐ� =====*/

	std::shared_ptr<TextureBuffer> m_texBuffer;	// �e�N�X�`��
	float m_angle;								// ���݂̃A���O��
	float m_easingTimer;						// �C�[�W���O�Ɏg�p����^�C�}�[
	float m_alpha;								// �A���t�@�l�B
	const float SIDE_ALPHA = 0.2f;				// ��A���ɂ���Ƃ��̃A���t�@�l
	const float ADD_EASING_TIMER = 0.1f;		// �C�[�W���O�̉��Z�ʁB

public:

	enum class STATUS {

		UP,
		MIDDLE,
		DOWN,

	};

	enum class UI_STATUS {

		TUTORIAL,
		GAME,
		EXIT,

	};

private:

	STATUS m_status;
	STATUS m_nextStatus;
	STATUS m_defStatus;
	UI_STATUS m_UIStatus;

	// ��]��
	const float ROTATE_REVOLVER_UI = DirectX::XM_PIDIV4;
	// ��]�ʂ̏����l�B
	const std::array<float, 3> DEF_ROTATE = { DirectX::XM_2PI - ROTATE_REVOLVER_UI, 0.0f , ROTATE_REVOLVER_UI , };

public:

	/*===== �����o�֐� =====*/

	TitleUI(std::string TexPath, STATUS Status, UI_STATUS UIStatus);
	void Init();
	void Update();
	void Draw(const Vec2<float>& RevolverPos);

	void TransUp();
	void TransDown();
	bool GetCanTrans() { return 1.0f <= m_easingTimer; };

private:

	void ChangeUpdateNowUp();
	void ChangeUpdateNowMiddle();
	void ChangeUpdateNowDown();
	void Trans(STATUS NextStatus);

};