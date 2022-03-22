#include "Framework.h"
#include "ShadowScene.h"

ShadowScene::ShadowScene()
{
	LightBuffer::Light* light = Environment::Get()->GetLight(0);
	light->type = LightType::POINT;
	light->position = { 0, 80, -30 };
	light->range = 500.0f;

	Create();

	shadow = new Shadow();
}

ShadowScene::~ShadowScene()
{
	delete skybox;
	delete quad;
	delete sword;
	delete hinata;
	delete sphere;

	delete shadow;
}

void ShadowScene::Update()
{
	quad->UpdateWorld();
	sword->UpdateWorld();
	hinata->Update();
	sphere->UpdateWorld();
}

void ShadowScene::PreRender()
{
	shadow->SetDepthRender();

	sword->GetReader()->SetShader(L"Shadow/ModelDepthMap.hlsl");
	hinata->GetReader()->SetShader(L"Shadow/AnimatorDepthMap.hlsl");
	sphere->GetMaterial()->SetShader(L"Shadow/ModelDepthMap.hlsl");

	sword->Render();
	hinata->Render();
	sphere->Render();
}

void ShadowScene::Render()
{
	skybox->Render();

	quad->Render();

	sword->GetReader()->SetShader(L"Lighting/ModelLighting.hlsl");
	hinata->GetReader()->SetShader(L"Lighting/AnimatorLighting.hlsl");
	sphere->GetMaterial()->SetShader(L"Lighting/ModelLighting.hlsl");

	sword->Render();
	hinata->Render();
	sphere->Render();
}

void ShadowScene::PostRender()
{
	shadow->PostRender();
}

void ShadowScene::GUIRender()
{
	shadow->GUIRender();
}

void ShadowScene::Create()
{
	skybox = new SkyBox("Textures/DDS/Snow_ENV.dds");

	quad = new Quad();
	quad->tag = "Quad";
	quad->GetMaterial()->SetDiffuseMap("Textures/Landscape/Floor.png");
	quad->GetMaterial()->SetShader(L"Lighting/ModelLighting.hlsl");
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
}
