#include "Framework.h"
#include "ShootingScene.h"

ShootingScene::ShootingScene()
{
	terrain = new Terrain();

	hinata = new Hinata();
	hinata->SetTerrain(terrain);

	CAM->SetTarget(hinata);
	CAM->LoadTargetMode();

	//
	RECT rect;
	POINT p1, p2;
	GetClientRect(hWnd, &rect);
	p1.x = rect.left;
	p1.y = rect.top;
	p2.x = rect.right;
	p2.y = rect.bottom;
	ClientToScreen(hWnd, &p1);
	ClientToScreen(hWnd, &p2);
	rect.left = p1.x;
	rect.top = p1.y;
	rect.right = p2.x;
	rect.bottom = p2.y;
	ClipCursor(&rect);
	SetCursorPos(rect.right - CENTER_X, rect.bottom - CENTER_Y);
	//ShowCursor(false);
	//

	cursor = new Quad(50, 50);
	cursor->GetMaterial()->SetDiffuseMap("Textures/UI/cursor.png");
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
