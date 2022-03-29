#include "Framework.h"
#include "DeferredScene.h"

DeferredScene::DeferredScene()
{
	Create();

	gBuffer = new GBuffer();

	material = new Material();
	material->SetShader(L"Deferred/DeferredLighting.hlsl");
	UINT vertices[4] = { 0,1,2,3 };
	vertexBuffer = new VertexBuffer(vertices, sizeof(UINT), 4);
}

DeferredScene::~DeferredScene()
{
	delete quad;
	delete sword;
	delete hinata;
	delete sphere;

	delete gBuffer;
	
	delete material;
	delete vertexBuffer;
}

void DeferredScene::Update()
{
	quad->UpdateWorld();
	sword->UpdateWorld();
	hinata->Update();
	sphere->UpdateWorld();
}

void DeferredScene::PreRender()
{
	gBuffer->PreRender();

	quad->Render();
	sword->Render();
	hinata->Render();
	sphere->Render();
}

void DeferredScene::Render()
{
	Environment::Get()->LightRender();

	gBuffer->Render();

	vertexBuffer->IASet(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	material->Set();

	DC->Draw(4, 0);
}

void DeferredScene::PostRender()
{
	gBuffer->PostRender();
}

void DeferredScene::GUIRender()
{
}

void DeferredScene::Create()
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

	quad->GetMaterial()->SetShader(L"Deferred/ModelGBuffer.hlsl");
	sword->GetReader()->SetShader(L"Deferred/ModelGBuffer.hlsl");
	hinata->GetReader()->SetShader(L"Deferred/AnimGBuffer.hlsl");
	sphere->GetMaterial()->SetShader(L"Deferred/ModelGBuffer.hlsl");
}
