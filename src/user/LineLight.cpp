#include"LineLight.h"
#include"../engine/DrawFunc3D.h"

LineLight::LineLight(const std::vector<Vec3<float>> &POS_ARRAY)
{
	model = Importer::Instance()->LoadModel("resource/user/", "block.glb");
	for (int i = 0; i < model->m_meshes.size(); ++i)
	{
		model->m_meshes[i].material->constData.lambert.emissive = { 1.0f,1.0f,1.0f };
		model->m_meshes[i].material->constData.lambert.emissiveFactor = 1.0f;
		model->m_meshes[i].material->Mapping();
	}

	lightBlocks.reserve(POS_ARRAY.size());
	lightBlocks.resize(POS_ARRAY.size());
	for (int i = 0; i < POS_ARRAY.size(); ++i)
	{
		lightBlocks[i] = std::make_unique<LightBlock>(POS_ARRAY[i], model);
	}
}

void LineLight::Init()
{
	for (int i = 0; i < lightBlocks.size(); ++i)
	{
		lightBlocks[i]->Init();
	}
}

void LineLight::Update()
{
	for (int i = 0; i < lightBlocks.size(); ++i)
	{
		lightBlocks[i]->Update();
	}
}

void LineLight::Draw(Camera &CAMERA)
{
	for (int i = 0; i < lightBlocks.size(); ++i)
	{
		lightBlocks[i]->Draw(CAMERA);
	}
}