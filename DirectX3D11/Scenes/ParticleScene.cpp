#include "Framework.h"
#include "ParticleScene.h"

ParticleScene::ParticleScene()
{
	//particle = new Spark("Textures/Particle/star.png", true);
	particle = new Rain();
}

ParticleScene::~ParticleScene()
{
	delete particle;
}

void ParticleScene::Update()
{
	if (KEY_DOWN('1'))
		particle->Play(Vector3());

	particle->Update();
}

void ParticleScene::PreRender()
{
}

void ParticleScene::Render()
{
	particle->Render();
}

void ParticleScene::PostRender()
{
}

void ParticleScene::GUIRender()
{
	particle->GUIRender();
}
