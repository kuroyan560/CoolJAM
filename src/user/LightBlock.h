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
	LightBlock(std::shared_ptr<Model> MODEL);
	void Init(const Vec3<float> &POS);
	void Update();
	void Draw(Camera &CAMERA);

	void Stop();

private:
	std::shared_ptr<Model> m_model;
	Transform m_tranform;

	Vec3<float> m_pos;
	Vec3<float>	m_vel;
	const float M_MAX_HIGHT;
	int m_timer;

	bool m_stopFlag;
	bool m_finishFlag;
};