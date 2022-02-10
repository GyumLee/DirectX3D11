#include "Framework.h"
#include "ShootingScene.h"

ShootingScene::ShootingScene()
{
	terrain = new Terrain();

	hinata = new Hinata();
	hinata->SetTerrain(terrain);

	ninja = new Ninja();
	ninja->SetTerrain(terrain);
	ninja->SetTarget(hinata);

	CAM->SetTarget(hinata);
	CAM->LoadTargetMode();
	
	cursor = new Quad(50, 50);
	cursor->GetMaterial()->SetDiffuseMap("Textures/UI/Crosshair.png");
	cursor->position = { CENTER_X, CENTER_Y, 0.0f };

	BulletManager::Get()->CreateBullets();
}

ShootingScene::~ShootingScene()
{
	delete terrain;

	delete hinata;
	delete ninja;

	delete cursor;

	BulletManager::Delete();
}

void ShootingScene::Update()
{
	//cursor->position.x = mousePos.x;
	//cursor->position.y = WIN_HEIGHT - mousePos.y;

	hinata->Update();
	ninja->Update();

	cursor->UpdateWorld();

	BulletManager::Get()->Update();
}

void ShootingScene::PreRender()
{
}

void ShootingScene::Render()
{
	terrain->Render();

	hinata->Render();
	ninja->Render();

	BulletManager::Get()->Render();
}

void ShootingScene::PostRender()
{
	cursor->Render();

	ninja->PostRender();
}

void ShootingScene::GUIRender()
{
	hinata->GUIRender();
	ninja->GUIRender();
}
