#pragma once
#include "Vec.h"
#include "D3D12App.h"
#include "D3D12Data.h"
#include "WinApp.h"
#include <string>
#include <memory>

class TutorialHexagon {

private:

	/*===== �����o�ϐ� =====*/

	std::shared_ptr<TextureBuffer> m_hexagonTex;	// �Z�p�`�̃e�N�X�`��

	float m_easingTimer;// �C�[�W���O�̃^�C�}�[
	const float ADD_EASING_TIMER = 0.05f;
	float m_rotate;		// ��]
	float m_scale;		// �g�嗦
	float m_alpha;		// �s�����x

	bool m_isExp;		// �g��t���O(���̃C�[�W���O�̂����A�g�債�Ă����Ԃ���\���B)
	bool m_isAppear;	// �o�������B
	bool m_isNormal;	// �ʏ��Ԃ��B
	bool m_isExit;		// ���Œ����B

	const float DEF_SCALE = 1.0f;
	const float EXP_SCALE = 1.8f;
	const float EASE_ROTATE = DirectX::XM_2PI * 1.0f;
	const Vec2<float> DRAW_POS = WinApp::Instance()->GetExpandWinCenter() + Vec2<float>(0.0f, 200.0f);


public:

	/*===== �����o�֐� =====*/

	TutorialHexagon(std::string File);
	void Init();
	void Update();
	void Draw();

	// �o�����Ă����Ԃ�
	bool GetIsAlive() { return m_isAppear || m_isNormal; }

	void Appear();
	void Exit();

};