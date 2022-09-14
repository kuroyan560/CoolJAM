#pragma once
#include "Vec.h"
#include "D3D12App.h"
#include "D3D12Data.h"
#include "WinApp.h"
#include <string>
#include <memory>
#include <array>

class GameStartTimer {
	/*===== �����o�ϐ� =====*/

	std::shared_ptr<TextureBuffer> m_timerTexture;	// �^�C�}�[�̃e�N�X�`��
	const int m_se;

	Vec2<float> m_timerPos;

	Vec2<float> m_offsetPos;

	float m_easingTimer;	// �C�[�W���O�̃^�C�}�[

	int exitTransitionTimer;	// ���S�ɂƂǂ܂鎞�ԁB
	const int EXIT_TRANSITION_TIMER = 10;

	bool m_isAppear;		// �o�������B
	bool m_isExit;			// ���Œ����B
	bool m_isActive;
	bool m_isGoTexture;

	const float DEF_NUM_SCALE = 1.0f;
	const float DEF_GO_SCALE = 1.5f;
	const Vec2<float> MIDDLE_POS = WinApp::Instance()->GetExpandWinCenter();
	const Vec2<float> APPEAR_POS = Vec2<float>(WinApp::Instance()->GetExpandWinCenter().x, WinApp::Instance()->GetExpandWinSize().y + WinApp::Instance()->GetExpandWinCenter().y);
	const Vec2<float> EXIT_POS = Vec2<float>(WinApp::Instance()->GetExpandWinCenter().x, -WinApp::Instance()->GetExpandWinCenter().y);

public:

	/*===== �����o�֐� =====*/

	GameStartTimer(std::shared_ptr<TextureBuffer> TexBuffer, const bool& IsGoTexture,const int& SE);
	void Init();
	void Update(const Vec2<float> &OffsetPos, const float &AddEasingTimer);
	void Draw();

	// �o�����Ă����Ԃ�
	bool GetIsActive() { return m_isActive; }
	bool GetIsStart() { return m_isAppear; }
	bool GetIsEnd() { return m_isExit && 1.0f <= m_easingTimer; }

	void Start();

};


class GameStartTimerUI {

private:

	/*===== �����o�ϐ� =====*/

	std::array<std::shared_ptr<GameStartTimer>, 4> m_timer;	// �^�C�}�[�N���X�B

	Vec2<float> m_offsetPos;

	int m_countTimer;
	const int COUNT_TIMER = 60;

	bool m_isActive;
	bool m_isStartGo;	// Go�̃e�N�X�`�����X�^�[�g���������B

	int m_countUpSE;
	int m_countFinishSE;

	bool m_startFlag;
public:

	/*===== �����o�֐� =====*/

	GameStartTimerUI(std::array<std::shared_ptr<TextureBuffer>, 4> TIME);
	void Init();
	void Update(const Vec2<float> &OffsetPos, const float &AddEasingTimer);
	void Draw();

	// �o�����Ă����Ԃ�
	bool GetIsActive() { return m_isActive; }

	void Start();

	bool IsStart() { return m_startFlag; }
};