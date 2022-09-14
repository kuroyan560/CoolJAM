#pragma once
#include "D3D12App.h"
#include "KuroEngine.h"
#include "Vec.h"
#include <memory>
#include <array>

class PlayerArrow {

private:

	/*===== ƒƒ“ƒo•Ï” ======*/

	std::shared_ptr<TextureBuffer> m_texBuffer;


	const float ARROW_SPAN = 10.0f;


public:

	PlayerArrow();

	void Draw(const Vec3<float>& PlayerPos, const Vec3<float>& MousePos);


};
