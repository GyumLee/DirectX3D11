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
	if (KEY_DOWN('1'))
		ninja->PlayClip(0);
	if (KEY_DOWN('2'))
		ninja->PlayClip(1);
	if (KEY_DOWN('3'))
		ninja->PlayClip(2);

	if (KEY_DOWN(VK_ESCAPE))
		ninja->StopClip();

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
