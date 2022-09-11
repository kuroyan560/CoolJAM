#include "Vec.h"
#include "Transform.h"
#include "EnemyWave.h"
#include "Model.h"
#include <memory>
#include <vector>
#include "Importer.h"
#include"LightBlock.h"

class LineLight
{
public:
	LineLight(const std::vector<Vec3<float>*> &POS_ARRAY);
	void Init();
	void Update();
	void Draw(Camera &CAMERA);

	void Stop();
private:
	std::shared_ptr<Model>model;
	std::vector<std::unique_ptr<LightBlock>>lightBlocks;
	std::vector<Vec3<float>*>lightBlockPosArray;
};