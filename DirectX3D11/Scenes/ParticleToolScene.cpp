#include "Framework.h"
#include "ParticleToolScene.h"

ParticleToolScene::ParticleToolScene()
{
	particleSystem = new ParticleSystem();
	particleSystem->Play(Vector3());
}

ParticleToolScene::~ParticleToolScene()
{
	delete particleSystem;
}

void ParticleToolScene::Update()
{
	if (KEY_DOWN('1'))
		particleSystem->Save("TextData/Particles/Test.fx");

	if (KEY_DOWN('2'))
		particleSystem->Load("TextData/Particles/Test.fx");

	particleSystem->Update();
}

void ParticleToolScene::PreRender()
{
}

void ParticleToolScene::Render()
{
	particleSystem->Render();
}

void ParticleToolScene::PostRender()
{
}

void ParticleToolScene::GUIRender()
{
	particleSystem->GUIRender();

	if (ImGui::Button("Play"))
		particleSystem->Play(Vector3());
}
