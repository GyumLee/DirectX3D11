#include "Framework.h"
#include "TerrainScene.h"

TerrainScene::TerrainScene()
{
	terrain = new Terrain();
	terrain->GetMaterial()->Load("TextData/Materials/Dirt2.mat");

	sphere = new Sphere();
	sphere->GetMaterial()->Load("TextData/Materials/FieldStone.mat");
	sphere->Load();

	destPos = sphere->position;
}

TerrainScene::~TerrainScene()
{
	delete terrain;
	delete sphere;
}

void TerrainScene::Update()
{
	if (MOUSE_CLICK(0) && !ImGui::GetIO().WantCaptureMouse)
	{
		destPos = terrain->Picking();
	}

	Vector3 direction = destPos - sphere->position;

	if (direction.Length() > 1.0f)
	{
		sphere->position += direction.Normalize() * 30.0f * DELTA;
	}

	sphere->position.y = terrain->GetHeight(sphere->position);

	terrain->UpdateWorld();
	sphere->UpdateWorld();
}

void TerrainScene::PreRender()
{
}

void TerrainScene::Render()
{
	terrain->Render();
	sphere->Render();
}

void TerrainScene::PostRender()
{
}

void TerrainScene::GUIRender()
{
	sphere->GUIRender();

	sphere->GetMaterial()->GUIRender();
	terrain->GetMaterial()->GUIRender();
}
