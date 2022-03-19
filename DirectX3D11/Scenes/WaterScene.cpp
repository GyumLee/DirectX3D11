#include "Framework.h"
#include "WaterScene.h"

WaterScene::WaterScene()
{
	Create();

	//quad->GetMaterial()->SetShader(L"Water/Refraction.hlsl");
	//refraction = new Refraction("Textures/Landscape/Wave.dds");

	water = new Water("Textures/Landscape/Wave.dds");
}

WaterScene::~WaterScene()
{
	delete skybox;
	//delete quad;
	delete sword;
	delete hinata;
	delete sphere;

	delete water;
}

void WaterScene::Update()
{
	//quad->UpdateWorld();
	sword->UpdateWorld();
	hinata->Update();
	sphere->UpdateWorld();

	water->Update();
}

void WaterScene::PreRender()
{
	water->SetRefraction();

	skybox->Render();

	sword->Render();
	hinata->Render();
	sphere->Render();

	water->SetReflection();

	skybox->Render();

	sword->Render();
	hinata->Render();
	sphere->Render();
}

void WaterScene::Render()
{
	skybox->Render();

	//refraction->SetRender();
	//quad->Render();
	water->Render();

	sword->Render();
	hinata->Render();
	sphere->Render();
}

void WaterScene::PostRender()
{
	//refraction->PostRender();
}

void WaterScene::GUIRender()
{
	//quad->GUIRender();
	//refraction->GUIRender();
	water->GUIRender();
}

void WaterScene::Create()
{
	skybox = new SkyBox("Textures/DDS/Snow_ENV.dds");

	//quad = new Quad();
	//quad->tag = "Quad";
	//quad->GetMaterial()->SetDiffuseMap("Textures/Landscape/Floor.png");
	//quad->Load();

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

	//quad->GetMaterial()->SetShader(L"Lighting/ModelLighting.hlsl");
}
