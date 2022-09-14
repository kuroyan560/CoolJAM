#include "PlayerDamageEffect.h"
#include "SlowMgr.h"

PlayerDamageEffectMgr::PlayerDamageEffectMgr()
{

	/*===== �R���X�g���N�^ =====*/

	m_frameTexture = D3D12App::Instance()->GenerateTextureBuffer("resource/user/damageFrame.png");
	m_frameAlpha = 0.0f;

	m_damageEffectTimer = 0;

	for (auto& index : m_effect) {

		index.Init();

	}

	m_generateCount = 0;
	m_generateSpan = 0;

}

void PlayerDamageEffectMgr::Init()
{

	/*===== ���������� ======*/

	m_frameAlpha = 0.0f;

	m_damageEffectTimer = 0;

	for (auto& index : m_effect) {

		index.Init();

	}

	m_generateCount = 0;
	m_generateSpan = 0;

}

void PlayerDamageEffectMgr::Update()
{

	/*===== �X�V���� =====*/

	// �L��������Ă���Ԃ̃^�C�}�[��0��肤����������
	if (0 < m_damageEffectTimer) {

		// �t���[���̃A���t�@��1�ɋ߂Â���B
		m_frameAlpha += (1.0f - m_frameAlpha) / 5.0f * SlowMgr::Instance()->m_slow;

		// �^�C�}�[�����炵�ďI��点��B
		m_damageEffectTimer -= 1.0f * SlowMgr::Instance()->m_slow;

	}
	else {

		// �A���t�@��0�ɋ߂Â���B
		if (0 < m_frameAlpha) {

			m_frameAlpha -= m_frameAlpha / 10.0f * SlowMgr::Instance()->m_slow;

			// �A���t�@�����ȉ��ɂȂ�����0�ɂ���B
			if (m_frameAlpha < 0.01f) {

				m_frameAlpha = 0.0f;

			}

		}

	}

	// �A�Ԃ𐶐��B
	if (m_generateCount < GENERATE_COUNT) {

		m_generateSpan += 1.0f * SlowMgr::Instance()->m_slow;
		if (GENERATE_SPAN <= m_generateSpan) {

			m_generateSpan = 0;

			// ���݂̐������ɂ���Đ�������ʒu�����߂�B
			bool isHori = KuroFunc::GetRand(0, 1);
			bool isUpper = KuroFunc::GetRand(0, 1);
			bool isRight = KuroFunc::GetRand(0, 1);

			// ��������ʒu�B
			Vec2<float> generatePos;

			// ����(�㉺)�ɐ�������ꍇ
			if (isHori) {

				// �㉺�Ō��߂�B
				if (isUpper) {

					generatePos.y = KuroFunc::GetRand(0.0f, GENERATE_RANGE);

				}
				else {

					generatePos.y = KuroFunc::GetRand(WinApp::Instance()->GetExpandWinSize().y - GENERATE_RANGE, WinApp::Instance()->GetExpandWinSize().y);

				}

				// �������߂�B
				generatePos.x = KuroFunc::GetRand(0.0f, WinApp::Instance()->GetExpandWinSize().x);

			}
			// ����(���E)�ɐ�������ꍇ
			else {

				// ���E�Ō��߂�B
				if (isRight) {

					generatePos.x = KuroFunc::GetRand(0.0f, GENERATE_RANGE);

				}
				else {

					generatePos.x = KuroFunc::GetRand(WinApp::Instance()->GetExpandWinSize().x - GENERATE_RANGE, WinApp::Instance()->GetExpandWinSize().x);

				}

				// �������߂�B
				generatePos.y = KuroFunc::GetRand(0.0f, WinApp::Instance()->GetExpandWinSize().y);

			}


			// ��������B
			for (auto& index : m_effect) {

				if (index.GetIsActive()) continue;

				index.Generate(generatePos);

				break;

			}


			++m_generateCount;

		}

	}


	// �A�Ԃ̍X�V����
	for (auto& index : m_effect) {

		if (!index.GetIsActive()) continue;

		index.Update();

	}

}

#include "DrawFunc2D.h"
#include "WinApp.h"
void PlayerDamageEffectMgr::Draw()
{

	/*===== �`�揈�� =====*/

	DrawFunc2D::DrawRotaGraph2D(WinApp::Instance()->GetExpandWinCenter(), Vec2<float>(1.0f, 1.0f), 0.0f, m_frameTexture, m_frameAlpha);

	for (auto& index : m_effect) {

		if (!index.GetIsActive()) continue;

		index.Draw();

	}

}

void PlayerDamageEffectMgr::Start()
{

	/*===== �J�n���� =====*/

	m_damageEffectTimer = DAMAGE_EFFECT_TIMER;

	m_generateCount = 0;
	m_generateSpan = 0;

}

PlayerDamageEffect::PlayerDamageEffect()
{

	/*===== �R���X�g���N�^ =====*/

	D3D12App::Instance()->GenerateTextureBuffer(m_frameTexture.data(), "resource/user/damageEffect.png", 4, Vec2<int>(4, 1));
	m_animNumber = 0;
	m_animSpeed = 0;
	m_pos = Vec2<float>();
	m_isActive = false;
	m_isExit = false;
	m_alpha = 0;

}

void PlayerDamageEffect::Init()
{

	/*===== ���������� =====*/

	m_animNumber = 0;
	m_animSpeed = 0;
	m_pos = Vec2<float>();
	m_isActive = false;
	m_isExit = false;
	m_alpha = 0;

}

void PlayerDamageEffect::Generate(const Vec2<float>& Pos)
{

	/*===== �������� =====*/

	m_animNumber = 0;
	m_animSpeed = 0;
	m_pos = Pos;
	m_isActive = true;
	m_isExit = false;
	m_alpha = 0;

}

void PlayerDamageEffect::Update()
{

	/*===== �X�V���� ======*/

	// �A�j���[�V�������X�V�B
	m_animSpeed += 1.0f * SlowMgr::Instance()->m_slow;
	if (ANIM_SPEED <= m_animSpeed) {

		m_animSpeed = 0.0f;

		// �A�Ԃ��X�V�B
		++m_animNumber;
		if (static_cast<int>(m_frameTexture.size()) - 1 <= m_animNumber) {

			m_animNumber = static_cast<int>(m_frameTexture.size()) - 1;

			m_isExit = true;

		}

	}

	// �A���t�@���X�V�B
	if (!m_isExit) {

		m_alpha += (1.0f - m_alpha) / 5.0;

	}
	else {


		m_alpha -= m_alpha / 5.0;

		// �A���t�@�l�����l����������珉��������B
		if (m_alpha <= 0.01f) {

			Init();

		}


	}

}

void PlayerDamageEffect::Draw()
{

	/*===== �`�揈�� =====*/

	DrawFunc2D::DrawRotaGraph2D(m_pos, Vec2<float>(1.0f, 1.0f), 0.0f, m_frameTexture[m_animNumber], m_alpha);

}
