#pragma once
#include"../engine/Common/Vec.h"
#include"../engine/D3D12App.h"

class BonusEffect
{
public:
	BonusEffect();
	void Init();
	void Update();
	void Draw();

	void Start();
	void Stop();
private:
	Vec2<float>m_pos, m_size;
	std::shared_ptr<TextureBuffer>m_feverEffectTexBuffer;

	bool m_startFlag;
	bool m_stopFlag;
};
