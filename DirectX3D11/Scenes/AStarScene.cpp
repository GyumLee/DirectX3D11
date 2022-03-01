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

	//CAM->SetTarget(character);

	CreateObstacles();
}

AStarScene::~AStarScene()
{
	delete terrain;
	delete character;
	delete aStar;

	for (Obstacle* obstacle : obstacles)
		delete obstacle;
}

void AStarScene::Update()
{
	character->Update();
	aStar->Update();

	for (Obstacle* obstacle : obstacles)
		obstacle->Update();
}

void AStarScene::PreRender()
{
}

void AStarScene::Render()
{
	terrain->Render();
	character->Render();
	aStar->Render();

	for (Obstacle* obstacle : obstacles)
		obstacle->Render();
}

void AStarScene::PostRender()
{
}

void AStarScene::GUIRender()
{
	character->GUIRender();

	for (Obstacle* obstacle : obstacles)
		obstacle->GUIRender();
}

void AStarScene::CreateObstacles()
{
	for (UINT i = 0; i < 5; i++)
	{
		Obstacle* obstacle = new Obstacle("TV", Collider::BOX);
		obstacle->tag = "TV_" + to_string(i);
		obstacle->Load();
		obstacle->Update();
		obstacles.push_back(obstacle);

		aStar->AddObstacle(obstacle->GetCollider());

		LightBuffer::Light* light = Environment::Get()->AddLight();
		light->type = LightType::SPOT;
		light->position = obstacle->GlobalPos();
		light->direction = { 0, -1, 0 };
		light->range = 70;
	}

	aStar->SetObstacleNode();
}
