#include "FeverGauge.h"
#include "D3D12App.h"
#include "SlowMgr.h"

FeverGauge::FeverGauge()
{

	/*===== �R���X�g���N�^ =====*/

	m_gaugeTexture = D3D12App::Instance()->GenerateTextureBuffer("resource/user/UI/fever_gauge/frame.png");
	m_baseRate = 0.0f;
	m_nowRate = 0.0f;
	m_isActive = false;

}

void FeverGauge::Init()
{

	/*===== ���������� =====*/

	m_baseRate = 0.0f;
	m_nowRate = 0.0f;
	m_isActive = false;

}

void FeverGauge::Update(const bool& IsActive, const float& Rate)
{

	/*===== �X�V���� =====*/

	m_isActive = IsActive;

	// �L��������Ă��Ȃ������珈�����΂��B
	if (!m_isActive) return;

	// ��Ԑ�̃��[�g��ݒ�B
	m_baseRate = Rate;

	// �⊮����B
	m_nowRate += (m_baseRate - m_nowRate) / 10.0f * SlowMgr::Instance()->m_slow;


}

#include "DrawFunc2D.h"
void FeverGauge::Draw()
{

	/*===== �`�揈�� =====*/

	if (!m_isActive) return;

	Vec2<float> centerPos = Vec2<float>(1280.0f / 2.0f, 32.0f);
	float texHeight = 8.0f;
	DrawFunc2D::DrawExtendGraph2D(centerPos - Vec2<float>(m_nowRate * GAUGE_LENGTH, texHeight), centerPos + Vec2<float>(m_nowRate * GAUGE_LENGTH, texHeight), m_gaugeTexture);

}
