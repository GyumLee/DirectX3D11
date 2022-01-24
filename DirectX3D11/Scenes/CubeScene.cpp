#include "Framework.h"
#include "CubeScene.h"

CubeScene::CubeScene()
{
	cube = new Cube();
	cube->GetMaterial()->SetDiffuseMap("Landscape/box.png");

	cube1 = new Cube();
	cube1->tag = "Cube1";
	cube1->GetMaterial()->SetDiffuseMap("Landscape/Stones.png");

	cube1->SetParent(cube);
}

CubeScene::~CubeScene()
{
	delete cube;
	delete cube1;
}

void CubeScene::Update()
{
	if (KEY_DOWN('1'))
		SceneManager::Get()->Add("Grid");
	if (KEY_DOWN('2'))
		SceneManager::Get()->Remove("Grid");
	if (KEY_DOWN('1'))
		SceneManager::Get()->Add("Plane");

	if (KEY_PRESS('W'))
		cube->position.z += DELTA;
	else if (KEY_PRESS('S'))
		cube->position.z -= DELTA;

	if (KEY_PRESS('D'))
		cube->position.x += DELTA;
	else if (KEY_PRESS('A'))
		cube->position.x -= DELTA;

	if (KEY_PRESS('E'))
		cube->position.y += DELTA;
	else if (KEY_PRESS('Q'))
		cube->position.y -= DELTA;

	cube->UpdateWorld();
	cube1->UpdateWorld();
}

void CubeScene::PreRender()
{
}

void CubeScene::Render()
{
	cube->Render();
	cube1->Render();
}

void CubeScene::PostRender()
{
}

void CubeScene::GUIRender()
{
	cube->GUIRender();
	cube1->GUIRender();
}
