#include "PlayerDamageEffect.h"
#include "SlowMgr.h"

PlayerDamageEffectMgr::PlayerDamageEffectMgr()
{

	/*===== コンストラクタ =====*/

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

	/*===== 初期化処理 ======*/

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

	/*===== 更新処理 =====*/

	// 有効化されている間のタイマーが0よりうえだったら
	if (0 < m_damageEffectTimer) {

		// フレームのアルファを1に近づける。
		m_frameAlpha += (1.0f - m_frameAlpha) / 5.0f * SlowMgr::Instance()->m_slow;

		// タイマーを減らして終わらせる。
		m_damageEffectTimer -= 1.0f * SlowMgr::Instance()->m_slow;

	}
	else {

		// アルファを0に近づける。
		if (0 < m_frameAlpha) {

			m_frameAlpha -= m_frameAlpha / 10.0f * SlowMgr::Instance()->m_slow;

			// アルファが一定以下になったら0にする。
			if (m_frameAlpha < 0.01f) {

				m_frameAlpha = 0.0f;

			}

		}

	}

	// 連番を生成。
	if (m_generateCount < GENERATE_COUNT) {

		m_generateSpan += 1.0f * SlowMgr::Instance()->m_slow;
		if (GENERATE_SPAN <= m_generateSpan) {

			m_generateSpan = 0;

			// 現在の生成数によって生成する位置を決める。
			bool isHori = KuroFunc::GetRand(0, 1);
			bool isUpper = KuroFunc::GetRand(0, 1);
			bool isRight = KuroFunc::GetRand(0, 1);

			// 生成する位置。
			Vec2<float> generatePos;

			// 水平(上下)に生成する場合
			if (isHori) {

				// 上下で決める。
				if (isUpper) {

					generatePos.y = KuroFunc::GetRand(0.0f, GENERATE_RANGE);

				}
				else {

					generatePos.y = KuroFunc::GetRand(WinApp::Instance()->GetExpandWinSize().y - GENERATE_RANGE, WinApp::Instance()->GetExpandWinSize().y);

				}

				// 横を決める。
				generatePos.x = KuroFunc::GetRand(0.0f, WinApp::Instance()->GetExpandWinSize().x);

			}
			// 垂直(左右)に生成する場合
			else {

				// 左右で決める。
				if (isRight) {

					generatePos.x = KuroFunc::GetRand(0.0f, GENERATE_RANGE);

				}
				else {

					generatePos.x = KuroFunc::GetRand(WinApp::Instance()->GetExpandWinSize().x - GENERATE_RANGE, WinApp::Instance()->GetExpandWinSize().x);

				}

				// 横を決める。
				generatePos.y = KuroFunc::GetRand(0.0f, WinApp::Instance()->GetExpandWinSize().y);

			}


			// 生成する。
			for (auto& index : m_effect) {

				if (index.GetIsActive()) continue;

				index.Generate(generatePos);

				break;

			}


			++m_generateCount;

		}

	}


	// 連番の更新処理
	for (auto& index : m_effect) {

		if (!index.GetIsActive()) continue;

		index.Update();

	}

}

#include "DrawFunc2D.h"
#include "WinApp.h"
void PlayerDamageEffectMgr::Draw()
{

	/*===== 描画処理 =====*/

	DrawFunc2D::DrawRotaGraph2D(WinApp::Instance()->GetExpandWinCenter(), Vec2<float>(1.0f, 1.0f), 0.0f, m_frameTexture, m_frameAlpha);

	for (auto& index : m_effect) {

		if (!index.GetIsActive()) continue;

		index.Draw();

	}

}

void PlayerDamageEffectMgr::Start()
{

	/*===== 開始処理 =====*/

	m_damageEffectTimer = DAMAGE_EFFECT_TIMER;

	m_generateCount = 0;
	m_generateSpan = 0;

}

PlayerDamageEffect::PlayerDamageEffect()
{

	/*===== コンストラクタ =====*/

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

	/*===== 初期化処理 =====*/

	m_animNumber = 0;
	m_animSpeed = 0;
	m_pos = Vec2<float>();
	m_isActive = false;
	m_isExit = false;
	m_alpha = 0;

}

void PlayerDamageEffect::Generate(const Vec2<float>& Pos)
{

	/*===== 生成処理 =====*/

	m_animNumber = 0;
	m_animSpeed = 0;
	m_pos = Pos;
	m_isActive = true;
	m_isExit = false;
	m_alpha = 0;

}

void PlayerDamageEffect::Update()
{

	/*===== 更新処理 ======*/

	// アニメーションを更新。
	m_animSpeed += 1.0f * SlowMgr::Instance()->m_slow;
	if (ANIM_SPEED <= m_animSpeed) {

		m_animSpeed = 0.0f;

		// 連番を更新。
		++m_animNumber;
		if (static_cast<int>(m_frameTexture.size()) - 1 <= m_animNumber) {

			m_animNumber = static_cast<int>(m_frameTexture.size()) - 1;

			m_isExit = true;

		}

	}

	// アルファを更新。
	if (!m_isExit) {

		m_alpha += (1.0f - m_alpha) / 5.0;

	}
	else {


		m_alpha -= m_alpha / 5.0;

		// アルファ値が一定値を下回ったら初期化する。
		if (m_alpha <= 0.01f) {

			Init();

		}


	}

}

void PlayerDamageEffect::Draw()
{

	/*===== 描画処理 =====*/

	DrawFunc2D::DrawRotaGraph2D(m_pos, Vec2<float>(1.0f, 1.0f), 0.0f, m_frameTexture[m_animNumber], m_alpha);

}
