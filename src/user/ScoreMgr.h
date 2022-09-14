#pragma once
#include "Singleton.h"
#include "Vec.h"
#include "WinApp.h"
#include "D3D12App.h"
#include <array>
#include <memory>

class ScoreMgr : public Singleton<ScoreMgr> {

private:

	/*===== �����o�ϐ� =====*/

	Vec2<float> m_pos;

	float m_easingTimer;	// �C�[�W���O�̃^�C�}�[

	bool m_isAppear;		// �o�������B
	bool m_isExit;			// ���Œ����B
	bool m_isCenter;
	bool m_isActive;

	int m_prevScore;		// 1F�O�̃X�R�A ���o�p
	int m_score;			// ���݂̃X�R�A
	int m_addScoreRegister;	// �X�R�A�ɉ��Z����ʁB�X�R�A�����Z����ۂ͈�x���̒l�ɓ����B�X�R�A�����炪�瑝���鉉�o�̂��߁B

	const float FONT_OFFSET = 64.0f;

	float m_scoreScale;
	const float DEF_SCALE = 0.5f;

	std::shared_ptr<TextureBuffer> m_ScoreTexture;

	const Vec2<float> MIDDLE_POS = WinApp::Instance()->GetExpandWinCenter() + Vec2<float>(0, 300.0f);
	const Vec2<float> CENTER_POS = WinApp::Instance()->GetExpandWinCenter();
	const Vec2<float> APPEAR_POS = Vec2<float>(WinApp::Instance()->GetExpandWinCenter().x, WinApp::Instance()->GetExpandWinSize().y + WinApp::Instance()->GetExpandWinCenter().y);
	const Vec2<float> EXIT_POS = Vec2<float>(WinApp::Instance()->GetExpandWinCenter().x, -WinApp::Instance()->GetExpandWinCenter().y);


public:

	/*===== �����o�֐� =====*/

	void Init();
	void Update(const Vec2<float>& Offset, const Vec2<float>& CenterOffset, const float& AddEasingTimer);
	void Draw();

	// �o�����Ă����Ԃ�
	bool GetIsActive() { return m_isActive; }

	void Appear();
	void Exit();
	void Center();	// Appear�ŏ���̈ʒu�Ɉړ���������ɓǂ�ł��������B

	void AddScore(const int& Score);	// �X�R�A�����Z����B
	void GameFinish();					// �ۑ����Ă���X�R�A����C�ɑ������B

};