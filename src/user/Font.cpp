#include "Font.h"

void Font::LoadFont()
{

	/*===== フォントをロード ======*/

	D3D12App::Instance()->GenerateTextureBuffer(m_allFont.data(), "resource/user/UI/num.png", 24, Vec2<int>(12, 2));

	for (auto& index : m_normalFont) {

		int indexCount = static_cast<int>(&index - &m_normalFont[0]);
		index = m_allFont[indexCount];

	}

	for (auto& index : m_stripeFont) {

		int indexCount = static_cast<int>(&index - &m_stripeFont[0]) + 12.0f;
		index = m_allFont[indexCount];

	}

}
