#include "Framework.h"
#include "TerrainLODScene.h"

TerrainLODScene::TerrainLODScene()
{
	terrain = new TerrainLOD("Textures/HeightMaps/HeightMap256.png");

	rsState = new RasterizerState();
	rsState->FillMode(D3D11_FILL_WIREFRAME);
	rsState->SetState();
}

TerrainLODScene::~TerrainLODScene()
{
	delete terrain;
	delete rsState;
}

void TerrainLODScene::Update()
{
	terrain->Update();
}

void TerrainLODScene::PreRender()
{
}

void TerrainLODScene::Render()
{
	terrain->Render();
}

void TerrainLODScene::PostRender()
{
}

void TerrainLODScene::GUIRender()
{
	terrain->GUIRender();
}
