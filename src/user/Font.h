#pragma once
#include "Singleton.h"
#include "D3D12App.h"
#include "D3D12Data.h"
#include <array>
#include <memory>

class Font : public Singleton<Font> {

public:

	/*===== メンバ変数 =====*/

	std::array<std::shared_ptr<TextureBuffer>, 24> m_allFont;
	std::array<std::shared_ptr<TextureBuffer>, 12> m_normalFont;
	std::array<std::shared_ptr<TextureBuffer>, 12> m_stripeFont;



public:

	/*===== メンバ関数 ======*/

	void LoadFont();

};