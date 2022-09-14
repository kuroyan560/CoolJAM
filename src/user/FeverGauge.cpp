#include "FeverGauge.h"
#include "D3D12App.h"
#include "SlowMgr.h"

FeverGauge::FeverGauge()
{

	/*===== コンストラクタ =====*/

	m_gaugeTexture = D3D12App::Instance()->GenerateTextureBuffer("resource/user/UI/fever_gauge/frame.png");
	m_baseRate = 0.0f;
	m_nowRate = 0.0f;
	m_isActive = false;

	m_feverText[0] = D3D12App::Instance()->GenerateTextureBuffer("resource/user/UI/fever_gauge/f.png");
	m_feverText[1] = D3D12App::Instance()->GenerateTextureBuffer("resource/user/UI/fever_gauge/e.png");
	m_feverText[2] = D3D12App::Instance()->GenerateTextureBuffer("resource/user/UI/fever_gauge/v.png");
	m_feverText[3] = D3D12App::Instance()->GenerateTextureBuffer("resource/user/UI/fever_gauge/e.png");
	m_feverText[4] = D3D12App::Instance()->GenerateTextureBuffer("resource/user/UI/fever_gauge/r.png");
	m_feverText[5] = D3D12App::Instance()->GenerateTextureBuffer("resource/user/UI/fever_gauge/extension.png");

	for (auto& index : m_sinWave) {

		int indexCount = static_cast<int>(&index - &m_sinWave[0]);

		index = ADD_SIN_WAVE * -indexCount;

	}
	m_alpha = 0.0f;
	m_lissajousTimer = 0;

}

void FeverGauge::Init()
{

	/*===== 初期化処理 =====*/

	m_baseRate = 0.0f;
	m_nowRate = 0.0f;
	m_isActive = false;

	for (auto& index : m_sinWave) {

		int indexCount = static_cast<int>(&index - &m_sinWave[0]);

		index = ADD_SIN_WAVE * -indexCount;

	}
	m_alpha = 0.0f;
	m_lissajousTimer = 0;

}

void FeverGauge::Update(const bool& IsActive, const float& Rate)
{

	/*===== 更新処理 =====*/

	m_isActive = IsActive;

	// 有効化されていなかったら処理を飛ばす。
	if (!m_isActive)
	{

		// アルファを0に近づける。
		if (0 < m_alpha) {

			m_alpha -= m_alpha / 3.0f;

		}

		// アルファが限りなく0に近かったら0にする。
		if (m_alpha <= 0.01f) {

			m_alpha = 0;

		}

		return;

	}

	// リサージュ曲線用タイマーを更新
	m_lissajousTimer += 0.02f;

	// 補間先のレートを設定。
	m_baseRate = Rate;

	// 補完する。
	m_nowRate += (m_baseRate - m_nowRate) / 10.0f * SlowMgr::Instance()->m_slow;

	// サイン波を更新。
	for (auto& index : m_sinWave) {

		index += ADD_SIN_WAVE;

	}

	// アルファを1に近づける。
	m_alpha += (1.0f - m_alpha) / 3.0f;

}

#include "DrawFunc2D.h"
void FeverGauge::Draw()
{

	/*===== 描画処理 =====*/

	if (!m_isActive) return;

	// リサージュ曲線を求める。
	float lissajousMove = 5.0f;
	Vec2<float> lissajousCurve = Vec2<float>(cosf(1.0f * m_lissajousTimer) * lissajousMove, sinf(2.0f * m_lissajousTimer) * lissajousMove);

	Vec2<float> centerPos = Vec2<float>(1280.0f / 2.0f, 32.0f);
	float texHeight = 8.0f;
	DrawFunc2D::DrawExtendGraph2D(centerPos - Vec2<float>(m_nowRate * GAUGE_LENGTH, texHeight) + lissajousCurve, centerPos + Vec2<float>(m_nowRate * GAUGE_LENGTH, texHeight) + lissajousCurve, m_gaugeTexture, m_alpha);

	// FEVER!を描画
	const float FEVER_SIZE_X = 64.0f;
	const float FEVER_OFFSET_Y = 85.0f;
	const float FEVER_START_POS_X = centerPos.x - (FEVER_SIZE_X * 2.5f);
	for (auto& index : m_feverText) {

		int indexCount = static_cast<int>(&index - &m_feverText[0]);

		// サイン波の長さを求める。
		const float SIN_WAVE = sinf(m_sinWave[indexCount]) * 20;

		// サイン波によって回転する量を求める。
		float sinRad = sinf(m_sinWave[indexCount]) * 0.3f;

		DrawFunc2D::DrawRotaGraph2D(Vec2<float>(FEVER_START_POS_X + indexCount * FEVER_SIZE_X, FEVER_OFFSET_Y + SIN_WAVE) + lissajousCurve, Vec2<float>(1.0f, 1.0f), sinRad, index, m_alpha);

	}

}
