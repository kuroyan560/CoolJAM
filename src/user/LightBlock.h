#include "Vec.h"
#include "Transform.h"
#include "EnemyWave.h"
#include "Model.h"
#include <memory>
#include <array>
#include "Importer.h"

class LightBlock
{
public:
	LightBlock(const Vec3<float> &POS, std::shared_ptr<Model> MODEL);
	void Init();
	void Update();
	void Draw(Camera &CAMERA);

private:
	std::shared_ptr<Model> m_model;
	Transform m_tranform;

	Vec3<float>m_pos,m_vel;
	const float M_MAX_HIGHT;
	int m_timer;
};