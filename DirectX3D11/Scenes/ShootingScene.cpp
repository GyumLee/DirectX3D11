#include "Framework.h"
#include "ShootingScene.h"

ShootingScene::ShootingScene()
{
	terrain = new Terrain();

	hinata = new Hinata();
	hinata->SetTerrain(terrain);

	CAM->SetTarget(hinata);
	CAM->LoadTargetMode();

	cursor = new Quad(50, 50);
	cursor->GetMaterial()->SetDiffuseMap("Textures/UI/Crosshair.png");
	cursor->position = { CENTER_X, CENTER_Y, 0.0f };
}

ShootingScene::~ShootingScene()
{
	delete terrain;

	delete hinata;

	delete cursor;
}

void ShootingScene::Update()
{
	//cursor->position.x = mousePos.x;
	//cursor->position.y = WIN_HEIGHT - mousePos.y;

	hinata->Update();
	cursor->UpdateWorld();
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
	cursor->Render();
}

void ShootingScene::GUIRender()
{
	hinata->GUIRender();
}
