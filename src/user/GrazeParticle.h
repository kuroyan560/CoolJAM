#pragma once
#include"../engine/Common/Vec.h"
#include"../engine/KuroEngine.h"
#include<memory>

class Camera;
class GrazeParticle
{
public:
	GrazeParticle();
	void Init(const Vec3<float>& POS, const Vec3<float>& DIR, float SIZE);
	void Update(float MAP_SIZE);
	void Draw(Camera &CAMERA);

	bool IsAlive();
private:
	Vec3<float>m_pos, m_vel;
	Vec2<float>m_size, m_dispappearSpeed;
	const float SPEED;
	const int DISAPPEAR_TIME;

	bool initFlag;
	bool initWallFlag;


	std::shared_ptr<TextureBuffer>texBuff;

};

