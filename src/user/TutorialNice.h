#pragma once
#include "Vec.h"
#include "D3D12App.h"
#include "D3D12Data.h"
#include "WinApp.h"
#include <string>
#include <memory>

class TutorialNice {

private:

	/*===== �����o�ϐ� =====*/

	std::shared_ptr<TextureBuffer> m_niceTexture;	// NICE�̃e�N�X�`��

	float m_easingTimer;// �C�[�W���O�̃^�C�}�[
	const float ADD_EASING_TIMER = 0.05f;
	float m_scale;		// �g�嗦
	float m_alpha;		// �s�����x

	int m_exitTimer;	// ������܂ł̃^�C�}�[
	const int EXIT_TIMER = 60;

	bool m_isAppear;	// �o�������B
	bool m_isExit;		// ���Œ����B
	bool m_isActive;

	const float DEF_SCALE = 1.3f;
	const float EXP_SCALE = 4.0f;
	const Vec2<float> DRAW_POS = WinApp::Instance()->GetExpandWinCenter() + Vec2<float>(0.0f, 200.0f);


public:

	/*===== �����o�֐� =====*/

	TutorialNice(std::string File);
	void Init();
	void Update();
	void Draw();

	// �o�����Ă����Ԃ�
	bool GetIsActive() { return m_isActive; }
	bool GetIsLowAlpha() { return m_alpha < 0.2f; }
	bool GetIsExit() { return m_isExit; }

	void Appear();

};