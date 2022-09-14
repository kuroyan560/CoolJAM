#include "StageFloor.h"
#include"Object.h"
#include"KuroEngine.h"
#include"Model.h"
#include"Importer.h"

StageFloor::StageFloor()
{
	m_circleModel = Importer::Instance()->LoadModel("resource/user/map/", "stageCircle.glb");
	m_floorModel = Importer::Instance()->LoadModel("resource/user/map/", "stageFloor.glb");
	m_floorScreen = D3D12App::Instance()->GenerateRenderTarget(DXGI_FORMAT_R8G8B8A8_UNORM,
		Color(0.0f, 0.0f, 0.0f, 1.0f), Vec2<int>(512, 512), L"FloorScreen");
	m_transform.SetScale(150.0f);

	m_floorModel->m_meshes[0].material->texBuff[COLOR_TEX] = m_floorScreen;
	m_rotate = { 0.0f,-0.1f,0.0f };
}

void StageFloor::ClearScreen()
{
	KuroEngine::Instance()->Graphics().ClearRenderTarget(m_floorScreen);
}

void StageFloor::ScreenTargetSet(bool Clear)
{
	if (Clear)ClearScreen();

	KuroEngine::Instance()->Graphics().SetRenderTargets({ m_floorScreen });
}

#include"DrawFunc_Append.h"
void StageFloor::Draw()
{
	m_transform.SetRotate(m_rotate);
	DrawFunc_Append::DrawModel(m_circleModel, m_transform, RenderTargetSwitch(), false, true);
	DrawFunc_Append::DrawModel(m_floorModel, m_transform, RenderTargetSwitch(), false, false);
}