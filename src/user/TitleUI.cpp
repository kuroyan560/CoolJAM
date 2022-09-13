#include "TitleUI.h"
#include "D3D12App.h"

TitleUI::TitleUI(std::string TexPath, STATUS Status, UI_STATUS UIStatus) {

	/*===== コンストラクタ =====*/

	m_texBuffer = D3D12App::Instance()->GenerateTextureBuffer(TexPath);
	m_status = Status;
	m_nextStatus = Status;
	m_defStatus = Status;
	m_UIStatus = UIStatus;
	m_easingTimer = 1.0f;

	switch (Status)
	{
	case TitleUI::STATUS::UP:

		m_angle = DEF_ROTATE[0];
		m_alpha = SIDE_ALPHA;

		break;
	case TitleUI::STATUS::MIDDLE:

		m_angle = DEF_ROTATE[1];
		m_alpha = 1.0f;

		break;
	case TitleUI::STATUS::DOWN:

		m_angle = DEF_ROTATE[2];
		m_alpha = SIDE_ALPHA;

		break;
	default:
		break;
	}

}

void TitleUI::Init() {

	/*===== 初期化処理 =====*/

	m_status = m_defStatus;
	m_nextStatus = m_defStatus;
	m_easingTimer = 1.0f;

	switch (m_defStatus)
	{
	case TitleUI::STATUS::UP:

		m_angle = DEF_ROTATE[0];
		m_alpha = SIDE_ALPHA;

		break;
	case TitleUI::STATUS::MIDDLE:

		m_angle = DEF_ROTATE[1];
		m_alpha = 1.0f;

		break;
	case TitleUI::STATUS::DOWN:

		m_angle = DEF_ROTATE[2];
		m_alpha = SIDE_ALPHA;

		break;
	default:
		break;
	}

}

void TitleUI::Update() {

	/*===== 更新処理 =====*/

	// 今のステータスと次のステータスが違かったら。
	if (m_status != m_nextStatus) {

		// 現在上だったら。
		if (m_status == STATUS::UP) {

			ChangeUpdateNowUp();

		}
		// 現在真ん中だったら。
		else if (m_status == STATUS::MIDDLE) {

			ChangeUpdateNowMiddle();

		}
		// 現在下だったら。
		else if (m_status == STATUS::DOWN) {

			ChangeUpdateNowDown();

		}

	}

	// イージングが終わったら。
	if (1.0f <= m_easingTimer) {

		m_status = m_nextStatus;

	}

}

#include "DrawFunc2D.h"
void TitleUI::Draw(const Vec2<float>& RevolverPos) {

	/*===== 描画処理 =====*/

	// ベクトルを求める。
	Vec2<float> vec = Vec2<float>(cosf(m_angle), sinf(m_angle));

	// UIの種類によってX軸のずらし量をきめる。
	Vec2<float> zureX;
	switch (m_UIStatus)
	{
	case TitleUI::UI_STATUS::TUTORIAL:
		zureX.x = 200.0f;
		break;
	case TitleUI::UI_STATUS::GAME:
		zureX.x = 250.0f;
		break;
	case TitleUI::UI_STATUS::EXIT:
		zureX.x = 45.0f;
		break;
	default:
		break;
	}

	Vec2<float> centerPos = RevolverPos + zureX + vec * (100.0f + 200.0f);

	DrawFunc2D::DrawRotaGraph2D(centerPos, Vec2<float>(1.0f, 1.0f), 0, m_texBuffer, m_alpha);

}

void TitleUI::ChangeUpdateNowUp() {

	/*===== 現在のSTATUSが上だったら =====*/

	// タイマーを更新。
	m_easingTimer += ADD_EASING_TIMER;
	if (1.0f <= m_easingTimer) m_easingTimer = 1.0f;

	// 次のステータスが下だったら。
	if (m_nextStatus == STATUS::DOWN) {

		// タイマーが半分以下だったら、アルファ値を消して移動させる。
		if (m_easingTimer < 0.5f) {

			m_alpha = SIDE_ALPHA - ((m_easingTimer * 2.0f) * SIDE_ALPHA);
			m_angle = DEF_ROTATE[0] - ((m_easingTimer * 2.0f) * ROTATE_REVOLVER_UI);

		}
		else {

			m_alpha = (m_easingTimer * 1.0f) * SIDE_ALPHA;
			m_angle = DEF_ROTATE[2] + (ROTATE_REVOLVER_UI - (m_easingTimer)*ROTATE_REVOLVER_UI);

		}

	}
	// 次のステータスが真ん中だったら。
	else if (m_nextStatus == STATUS::MIDDLE) {

		// 移動させる。
		m_alpha = SIDE_ALPHA + (m_easingTimer * (1.0f - SIDE_ALPHA));
		m_angle = DEF_ROTATE[0] + ROTATE_REVOLVER_UI * m_easingTimer;

	}

}

void TitleUI::ChangeUpdateNowMiddle() {

	/*===== 現在のSTATUSが真ん中だったら =====*/

	// タイマーを更新。
	m_easingTimer += ADD_EASING_TIMER;
	if (1.0f <= m_easingTimer) m_easingTimer = 1.0f;

	// 次のステータスが上だったら。
	if (m_nextStatus == STATUS::UP) {

		m_alpha = 1.0f - (m_easingTimer * (1.0f - SIDE_ALPHA));
		m_angle = DirectX::XM_2PI - (m_easingTimer * ROTATE_REVOLVER_UI);

	}
	// 次のステータスが下だったら。
	else if (m_nextStatus == STATUS::DOWN) {

		m_alpha = 1.0f - (m_easingTimer * (1.0f - SIDE_ALPHA));
		m_angle = m_easingTimer * ROTATE_REVOLVER_UI;

	}

}

void TitleUI::ChangeUpdateNowDown() {

	/*===== 現在のSTATUSが下だったら =====*/

	// タイマーを更新。
	m_easingTimer += ADD_EASING_TIMER;
	if (1.0f <= m_easingTimer) m_easingTimer = 1.0f;

	// 次のステータスが上だったら。
	if (m_nextStatus == STATUS::UP) {

		// タイマーが半分以下だったら、アルファ値を消して移動させる。
		if (m_easingTimer < 0.5f) {

			m_alpha = SIDE_ALPHA - ((m_easingTimer * 2.0f) * SIDE_ALPHA);
			m_angle = DEF_ROTATE[2] + ((m_easingTimer * 2.0f) * ROTATE_REVOLVER_UI);

		}
		else {

			m_alpha = (m_easingTimer * 1.0f) * SIDE_ALPHA;
			m_angle = DEF_ROTATE[0] + (-ROTATE_REVOLVER_UI + (m_easingTimer)*ROTATE_REVOLVER_UI);

		}

	}
	// 次のステータスが真ん中だったら。
	else if (m_nextStatus == STATUS::MIDDLE) {

		// 移動させる。
		m_alpha = SIDE_ALPHA + (m_easingTimer * (1.0f - SIDE_ALPHA));
		m_angle = DEF_ROTATE[2] - ROTATE_REVOLVER_UI * m_easingTimer;

	}

}

void TitleUI::Trans(STATUS Status) {

	/*===== 次のステータスを決める =====*/

	// 今のステータスが真ん中だったら。
	if (m_status == STATUS::MIDDLE) {

		// 次のステータスが上だったら。
		if (Status == STATUS::UP) {

			m_angle = DirectX::XM_2PI;

		}
		// 次のステータスが下だったら。
		else if (Status == STATUS::DOWN) {

			m_angle = 0.0f;

		}

	}

	m_nextStatus = Status;

	m_easingTimer = 0.0f;

}

void TitleUI::TransUp() {

	/*===== 上に遷移する場合 =====*/

	// 現在が上だったら。
	if (m_status == STATUS::UP) {

		Trans(STATUS::DOWN);

	}
	else if (m_status == STATUS::MIDDLE) {

		Trans(STATUS::UP);

	}
	else if (m_status == STATUS::DOWN) {

		Trans(STATUS::MIDDLE);

	}

}

void TitleUI::TransDown() {

	/*===== 下に遷移する場合 =====*/

	// 現在が上だったら。
	if (m_status == STATUS::UP) {

		Trans(STATUS::MIDDLE);

	}
	else if (m_status == STATUS::MIDDLE) {

		Trans(STATUS::DOWN);

	}
	else if (m_status == STATUS::DOWN) {

		Trans(STATUS::UP);

	}

}