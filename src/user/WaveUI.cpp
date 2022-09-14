#include "WaveUI.h"
#include "Font.h"
#include "SlowMgr.h"

WaveUI::WaveUI(std::string File)
{

	/*===== �R���X�g���N�^ =====*/

	m_waveTexture = D3D12App::Instance()->GenerateTextureBuffer(File);
	m_easingTimer = 0;
	m_isActive = false;
	m_isExit = false;
	m_isAppear = false;
	m_wavePos = Vec2<float>();

}

void WaveUI::Init(const int& MaxWave)
{

	/*===== ���������� =====*/

	m_easingTimer = 0;
	m_isActive = false;
	m_isExit = false;
	m_isAppear = false;
	m_wavePos = Vec2<float>();

	// �ő吔��10������������B
	if (MaxWave < 10) {

		m_maxWaveCountIndex[0] = -1;
		m_maxWaveCountIndex[1] = MaxWave;

	}
	else {

		m_maxWaveCountIndex[0] = KuroFunc::GetSpecifiedDigitNum(MaxWave, 1);
		m_maxWaveCountIndex[1] = KuroFunc::GetSpecifiedDigitNum(MaxWave, 0);

	}

	m_nowWaveCountIndex[0] = 0;
	m_nowWaveCountIndex[0] = 0;

}

void WaveUI::Update(const int& NowWave, const Vec2<float>& OffsetPos, const Vec2<float>& CenterOffset, const float& AddEasingTimer)
{

	/*===== �X�V���� =====*/

	if (!m_isActive) return;

	// �o������������B
	if (m_isAppear) {

		// �C�[�W���O�ʂ����߂�B
		float easingAmount = KuroMath::Ease(Out, Cubic, m_easingTimer, 0.0f, 1.0f);

		// ���W��ݒ�B
		m_wavePos = APPEAR_POS + ((MIDDLE_POS + OffsetPos) - APPEAR_POS) * easingAmount;

	}
	// �ޏo����������B
	else if (m_isExit) {

		// �C�[�W���O�ʂ����߂�B
		float easingAmount = KuroMath::Ease(In, Cubic, m_easingTimer, 0.0f, 1.0f);

		// ��Ԍ��̍��W�B
		Vec2<float> basePos = MIDDLE_POS;
		if (m_isCenter) {

			basePos = CENTER_POS + OffsetPos + CenterOffset;

		}

		// ���W��ݒ�B
		m_wavePos = (basePos + OffsetPos) + (EXIT_POS - (basePos + OffsetPos)) * easingAmount;

	}
	// �����ڍs��Ԃ�������B
	else if (m_isCenter) {

		// �C�[�W���O�ʂ����߂�B
		float easingAmount = KuroMath::Ease(InOut, Cubic, m_easingTimer, 0.0f, 1.0f);

		// ���W��ݒ�B
		m_wavePos = (MIDDLE_POS + OffsetPos) + ((CENTER_POS + OffsetPos + CenterOffset) - (MIDDLE_POS + OffsetPos)) * easingAmount;

	}

	m_easingTimer += AddEasingTimer * SlowMgr::Instance()->m_slow;
	if (1.0f <= m_easingTimer) {

		m_easingTimer = 1.0f;

	}

	// �ő吔��10������������B
	if (NowWave < 10) {

		m_nowWaveCountIndex[0] = -1;
		m_nowWaveCountIndex[1] = NowWave;

	}
	else {

		m_nowWaveCountIndex[0] = KuroFunc::GetSpecifiedDigitNum(NowWave, 1);
		m_nowWaveCountIndex[1] = KuroFunc::GetSpecifiedDigitNum(NowWave, 0);

	}

}

#include "DrawFunc2D.h"
void WaveUI::Draw(const AdjData &OFFSET_DATA)
{

	/*===== �`�揈�� =====*/

	if (!m_isActive) return;

	DrawFunc2D::DrawRotaGraph2D(m_wavePos + Vec2<float>(-250.0f, 0.0f)+ OFFSET_DATA.m_stringPos, Vec2<float>(1.0f, 1.0f) + Vec2<float>(OFFSET_DATA.m_stringSize, OFFSET_DATA.m_stringSize), 0, m_waveTexture);


	if (m_nowWaveCountIndex[0] != -1) {
		DrawFunc2D::DrawRotaGraph2D(m_wavePos + Vec2<float>(20.0f, 80.0f)+ OFFSET_DATA.m_numPos, Vec2<float>(1.25f, 1.25f) + Vec2<float>(OFFSET_DATA.m_numSize, OFFSET_DATA.m_numSize), 0, Font::Instance()->m_stripeFont[m_nowWaveCountIndex[0]]);
	}
	if (m_nowWaveCountIndex[1] != -1) {
		DrawFunc2D::DrawRotaGraph2D(m_wavePos + Vec2<float>(170.0f, 80.0f) + OFFSET_DATA.m_numPos, Vec2<float>(1.25f, 1.25f) + Vec2<float>(OFFSET_DATA.m_numSize, OFFSET_DATA.m_numSize), 0, Font::Instance()->m_stripeFont[m_nowWaveCountIndex[1]]);
	}

	DrawFunc2D::DrawRotaGraph2D(m_wavePos + Vec2<float>(300.0f, 120.0f) + OFFSET_DATA.m_numPos, Vec2<float>(0.5f, 0.5f) + Vec2<float>(OFFSET_DATA.m_numSize, OFFSET_DATA.m_numSize), 0, Font::Instance()->m_stripeFont[11]);

	if (m_maxWaveCountIndex[0] != -1) {
		DrawFunc2D::DrawRotaGraph2D(m_wavePos + Vec2<float>(380.0f, 120.0f) + OFFSET_DATA.m_numPos, Vec2<float>(0.5f, 0.5f) + Vec2<float>(OFFSET_DATA.m_numSize, OFFSET_DATA.m_numSize), 0, Font::Instance()->m_stripeFont[m_maxWaveCountIndex[0]]);
	}
	if (m_maxWaveCountIndex[1] != -1) {
		DrawFunc2D::DrawRotaGraph2D(m_wavePos + Vec2<float>(450.0f, 120.0f) + OFFSET_DATA.m_numPos, Vec2<float>(0.5f, 0.5f) + Vec2<float>(OFFSET_DATA.m_numSize, OFFSET_DATA.m_numSize), 0, Font::Instance()->m_stripeFont[m_maxWaveCountIndex[1]]);
	}

}

void WaveUI::Appear()
{

	/*===== �o������ =====*/

	m_isActive = true;
	m_isExit = false;
	m_isAppear = true;
	m_easingTimer = 0;

}

void WaveUI::Exit()
{

	/*===== ���ŏ��� =====*/

	m_isExit = true;
	m_isAppear = false;
	m_easingTimer = 0;

}

void WaveUI::Center()
{

	/*===== �����ڍs���� =====*/

	m_isActive = true;
	m_isAppear = false;
	m_isExit = false;
	m_isCenter = true;
	m_easingTimer = 0;

}
