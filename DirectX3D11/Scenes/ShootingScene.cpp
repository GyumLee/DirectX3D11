#include "Framework.h"
#include "ShootingScene.h"

ShootingScene::ShootingScene()
{
	terrain = new Terrain();

	hinata = new Hinata();
	hinata->SetTerrain(terrain);

	CAM->SetTarget(hinata);
}

ShootingScene::~ShootingScene()
{
	delete terrain;

	delete hinata;
}

void ShootingScene::Update()
{
	hinata->Update();
}

void ShootingScene::PreRender()
{
}

void ShootingScene::Render()
{
	terrain->Render();

	hinata->Render();
}

void ShootingScene::PostRender()
{
}

void ShootingScene::GUIRender()
{
	hinata->GUIRender();
}
