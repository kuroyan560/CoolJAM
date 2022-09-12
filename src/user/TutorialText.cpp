#include "TutorialText.h"

TutorialText::TutorialText() {

	/*===== コンストラクタ =====*/

	m_tutorialStatus = TUTORIAL_STATUS::MOUSE;
	m_floorRenderTarget = D3D12App::Instance()->GenerateRenderTarget(DXGI_FORMAT_R8G8B8A8_UNORM, Color(0, 0, 0, 0), Vec2<int>(512, 512), L"FloorScreen");

}

void TutorialText::Init() {

	/*===== 初期化処理 =====*/

	m_tutorialStatus = TUTORIAL_STATUS::MOUSE;

}

void TutorialText::Update() {

	/*===== 更新処理 =====*/

	switch (m_tutorialStatus)
	{
	case TutorialText::TUTORIAL_STATUS::MOUSE:

		/*-- マウスのUIのテクスチャをスクロール --*/

		break;
	case TutorialText::TUTORIAL_STATUS::DAMAGE_WALL:

		/*-- ダメージ壁の説明のUIのテクスチャをスクロール --*/

		break;
	case TutorialText::TUTORIAL_STATUS::ENEMY:

		/*-- 敵の説明のUIのテクスチャをスクロール --*/

		break;
	case TutorialText::TUTORIAL_STATUS::FEVER:

		/*-- フィーバーの説明のUIのテクスチャをスクロール --*/

		break;
	default:
		break;
	}

}

void TutorialText::Draw() {

	/*===== 描画処理 =====*/

}

void TutorialText::Next() {

	/*===== 次へ =====*/

	// フィーバーじゃなかったら。(フィーバーは最後のチュートリアルなので次はない。)
	if (m_tutorialStatus != TUTORIAL_STATUS::FEVER) {

		m_tutorialStatus = static_cast<TUTORIAL_STATUS>(static_cast<int>(m_tutorialStatus) + 1);

	}

}