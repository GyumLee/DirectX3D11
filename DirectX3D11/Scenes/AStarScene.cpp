#include "Framework.h"
#include "AStarScene.h"

AStarScene::AStarScene()
{
	terrain = new Terrain();

	aStar = new AStar();
	aStar->SetNode(terrain);

	character = new Character();
	character->SetTerrain(terrain);
	character->SetAStar(aStar);

	CAM->SetTarget(character);
}

AStarScene::~AStarScene()
{
	delete terrain;
	delete character;
	delete aStar;
}

void AStarScene::Update()
{
	character->Update();
	aStar->Update();
}

void AStarScene::PreRender()
{
}

void AStarScene::Render()
{
	terrain->Render();
	character->Render();
	aStar->Render();
}

void AStarScene::PostRender()
{
}

void AStarScene::GUIRender()
{
	character->GUIRender();
}
