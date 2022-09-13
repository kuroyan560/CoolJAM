#pragma once
#include "Vec.h"
#include "D3D12App.h"
#include "D3D12Data.h"
#include "WinApp.h"
#include <string>
#include <memory>

class WaveUI {

private:

	/*===== �����o�ϐ� =====*/

	std::shared_ptr<TextureBuffer> m_waveTexture;	// �uWAVE�v�̃e�N�X�`��

	Vec2<float> m_wavePos;

	float m_easingTimer;	// �C�[�W���O�̃^�C�}�[
	const float ADD_EASING_TIMER = 0.05f;

	bool m_isAppear;		// �o�������B
	bool m_isExit;			// ���Œ����B
	bool m_isActive;

	const float DEF_SCALE = 1.0f;
	const Vec2<float> MIDDLE_POS = WinApp::Instance()->GetExpandWinCenter() + Vec2<float>(0, 1.0f);
	const Vec2<float> APPEAR_POS = Vec2<float>(WinApp::Instance()->GetExpandWinCenter().x, WinApp::Instance()->GetExpandWinSize().y + WinApp::Instance()->GetExpandWinCenter().y) + Vec2<float>(0, 1.0f);
	const Vec2<float> EXIT_POS = Vec2<float>(WinApp::Instance()->GetExpandWinCenter().x, -WinApp::Instance()->GetExpandWinCenter().y) + Vec2<float>(0, 1.0f);


public:

	/*===== �����o�֐� =====*/

	WaveUI(std::string File);
	void Init();
	void Update();
	void Draw();

	// �o�����Ă����Ԃ�
	bool GetIsActive() { return m_isActive; }

	void Appear();
	void Exit();

};