#include "Framework.h"
#include "RenderTargetScene.h"

RenderTargetScene::RenderTargetScene()
{
	CreateModels();

	renderTarget = new RenderTarget();
	depthStencil = new DepthStencil();

	renderTexture = new RenderTexture(WIN_WIDTH * 0.5f, WIN_HEIGHT);
	renderTexture->SetSRV(renderTarget->GetSRV());
	renderTexture->position.x = WIN_WIDTH * 0.25f;
	renderTexture->position.y = WIN_HEIGHT * 0.5f;
	renderTexture->GetMaterial()->SetShader(L"PostEffect/Grayscale.hlsl");

	renderTexture2 = new RenderTexture(WIN_WIDTH * 0.5f, WIN_HEIGHT);
	renderTexture2->SetSRV(renderTarget->GetSRV());
	renderTexture2->position.x = WIN_WIDTH * 0.75f;
	renderTexture2->position.y = WIN_HEIGHT * 0.5f;
}

RenderTargetScene::~RenderTargetScene()
{
	delete sky;
	delete terrain;
	delete hinata;

	delete renderTarget;
	delete depthStencil;
	delete renderTexture;
	delete renderTexture2;
}

void RenderTargetScene::Update()
{
	hinata->Update();

	renderTexture->UpdateWorld();
	renderTexture2->UpdateWorld();
}

void RenderTargetScene::PreRender()
{
	renderTarget->Set(depthStencil);

	sky->Render();

	terrain->Render();
	hinata->Render();
}

void RenderTargetScene::Render()
{
	
}

void RenderTargetScene::PostRender()
{
	renderTexture->Render();
	renderTexture2->Render();
}

void RenderTargetScene::GUIRender()
{
	renderTexture->GUIRender();
}

void RenderTargetScene::CreateModels()
{
	sky = new SkyBox("Textures/DDS/ColdSunset.dds");

	terrain = new Terrain();

	hinata = new Hinata();
	hinata->SetTerrain(terrain);
}
