#include "Framework.h"
#include "CharacterScene.h"

CharacterScene::CharacterScene()
{
	ninja = new Ninja;
}

CharacterScene::~CharacterScene()
{
	delete ninja;
}

void CharacterScene::Update()
{
	ninja->Update();
}

void CharacterScene::PreRender()
{
}

void CharacterScene::Render()
{
	ninja->Render();
}

void CharacterScene::PostRender()
{
}

void CharacterScene::GUIRender()
{
	ninja->GUIRender();
}
