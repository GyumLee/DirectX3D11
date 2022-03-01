#include "Framework.h"
#include "LightScene.h"

LightScene::LightScene()
{
	Create();
}

LightScene::~LightScene()
{
	delete quad;
	delete sword;
	delete hinata;
	delete sphere;
}

void LightScene::Update()
{
	quad->UpdateWorld();
	sword->UpdateWorld();
	hinata->Update();
	sphere->UpdateWorld();
}

void LightScene::PreRender()
{
}

void LightScene::Render()
{
	quad->Render();
	sword->Render();
	hinata->Render();
	sphere->Render();

	Environment::Get()->LightRender();
}

void LightScene::PostRender()
{
}

void LightScene::GUIRender()
{
	//quad->GUIRender();
	//sword->GUIRender();
	//hinata->GUIRender();
	//sphere->GUIRender();
	quad->GetMaterial()->GUIRender();
	sphere->GetMaterial()->GUIRender();
}

void LightScene::Create()
{
	quad = new Quad();
	quad->tag = "Quad";
	quad->GetMaterial()->SetDiffuseMap("Textures/Landscape/Floor.png");
	quad->Load();

	sword = new Model("Sword");
	sword->tag = "TestSword";
	sword->Load();

	hinata = new ModelAnimator("Hinata");
	hinata->tag = "TestHinata";
	hinata->ReadClip("Run");
	hinata->PlayClip(0);
	hinata->Load();

	sphere = new Sphere();
	sphere->tag = "Sphere";
	sphere->GetMaterial()->Load("TextData/Materials/FieldStone.mat");
	sphere->Load();

	quad->GetMaterial()->SetShader(L"Lighting/ModelLighting.hlsl");
}
