#pragma once
#include "Vec.h"
#include "../engine/Model.h"

class Outline
{
public:
	Outline(std::shared_ptr<Model> MODEL, const Vec3<float> *POS, const Vec3<float> &SCALE, const Color &COLOR);
	void Init();
	void Upadte();
	void Draw(Camera &CAMERA);

private:
	Transform m_transform;
	std::shared_ptr<Model> m_model;
	const Vec3<float> *m_pos;
	Color m_color;
};

