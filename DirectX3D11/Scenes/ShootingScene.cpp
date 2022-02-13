#include "Framework.h"
#include "ShootingScene.h"

ShootingScene::ShootingScene()
{
	terrain = new Terrain();

	hinata = new Hinata();
	hinata->SetTerrain(terrain);

	CAM->LoadTargetMode();
	CAM->SetTarget(hinata);
	
	cursor = new Quad(50, 50);
	cursor->GetMaterial()->SetDiffuseMap("Textures/UI/Crosshair.png");
	cursor->position = { CENTER_X, CENTER_Y, 0.0f };

	BulletManager::Get()->CreateBullets();
	MonsterManager::Get()->CreateMonsters(terrain);
}

ShootingScene::~ShootingScene()
{
	delete terrain;

	delete hinata;

	delete cursor;

	BulletManager::Delete();
	MonsterManager::Delete();
}

void ShootingScene::Update()
{
	//cursor->position.x = mousePos.x;
	//cursor->position.y = WIN_HEIGHT - mousePos.y;

	hinata->Update();
	MonsterManager::Get()->Update();

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
	MonsterManager::Get()->Render();

	BulletManager::Get()->Render();
}

void ShootingScene::PostRender()
{
	cursor->Render();

	MonsterManager::Get()->PostRender();
}

void ShootingScene::GUIRender()
{
	hinata->GUIRender();
}
