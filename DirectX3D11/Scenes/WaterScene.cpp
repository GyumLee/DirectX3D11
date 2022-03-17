#include "Framework.h"
#include "WaterScene.h"

WaterScene::WaterScene()
{
	Create();
	quad->GetMaterial()->SetShader(L"Water/Reflection.hlsl");

	reflection = new Reflection(quad);
}

WaterScene::~WaterScene()
{
	delete skybox;
	delete quad;
	delete sword;
	delete hinata;
	delete sphere;

	delete reflection;
}

void WaterScene::Update()
{
	quad->UpdateWorld();
	sword->UpdateWorld();
	hinata->Update();
	sphere->UpdateWorld();

	reflection->Update();
}

void WaterScene::PreRender()
{
	reflection->SetReflection();

	skybox->Render();

	sword->Render();
	hinata->Render();
	sphere->Render();
}

void WaterScene::Render()
{
	skybox->Render();

	reflection->SetRender();
	quad->Render();

	sword->Render();
	hinata->Render();
	sphere->Render();
}

void WaterScene::PostRender()
{
	reflection->PostRender();
}

void WaterScene::GUIRender()
{
}

void WaterScene::Create()
{
	skybox = new SkyBox("Textures/DDS/ColdSunset.dds");

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
