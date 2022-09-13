#pragma once
#include "Vec.h"
#include "../engine/Model.h"

class Outline
{
public:
	Outline(std::shared_ptr<Model> MODEL, const Transform* TRANSFORM, const Color &COLOR);
	void Init();
	void Upadte();
	void Draw(Camera &CAMERA, bool HIT_FLAG);

private:
	const Transform *m_transform;
	std::shared_ptr<Model> m_model;
	Color m_color, m_hitColor;

};

