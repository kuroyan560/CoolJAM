#pragma once
#include "D3D12App.h"
#include "D3D12Data.h"
#include "Vec.h"
#include "WinApp.h"
#include <string>
#include <memory>

class TutorialText {

private:

	/*===== �����o�ϐ� =====*/

	std::shared_ptr<TextureBuffer> m_texture;		// �e�N�X�`��

	Vec2<float> m_pos;
	int m_appearNextDelay;		// �o�����������Ă��玟�Ɉȍ~�ł���悤�ɂȂ�܂ł̒x���B
	const int APPEAR_NEXT_DELAY = 30;
	bool m_isAppear;			// �o������
	bool m_isExit;				// �ޏo����
	bool m_isActive;			// �L��������Ă��邩
	bool m_isFinish;			// ���ׂĂ̏������I��������B
	float m_textureEasingTimer;	// �C�[�W���O�Ɏg�p����^�C�}�[
	const float ADD_TEXTURE_EASING_TIMER = 0.05f;

	const Vec2<float> START_POS = Vec2<float>(WinApp::Instance()->GetExpandWinCenter().x, WinApp::Instance()->GetExpandWinSize().y + WinApp::Instance()->GetExpandWinCenter().y);
	const Vec2<float> MIDDLE_POS = WinApp::Instance()->GetExpandWinCenter();
	const Vec2<float> END_POS = Vec2<float>(WinApp::Instance()->GetExpandWinCenter().x, -WinApp::Instance()->GetExpandWinCenter().y);


public:

	/*===== �����o�֐� =====*/

	TutorialText(std::string File);
	void Init();
	void Update();
	void Draw();

	bool GetIsActive() { return m_isActive; }
	bool GetisFinish() { return m_isFinish; }
	bool GetIsExit() { return m_isExit; }
	bool GetCanExit() { return APPEAR_NEXT_DELAY <= m_appearNextDelay; }
	float GetEasingTimer() { return m_textureEasingTimer; }

	void Activate();	// �L�������Đ^�񒆂ɓ������B
	void Exit();		// �^�񒆂����Ɉړ�������B�ړ����I�������L�����t���O��������B

};