#include "Framework.h"
#include "ModelAnimatorRenderScene.h"

ModelAnimatorRenderScene::ModelAnimatorRenderScene()
{
	string name = "Ninja";

	modelAnimator = new ModelAnimator(name);
	modelAnimator->ReadClip("Idle");
	modelAnimator->ReadClip("Run");
	modelAnimator->ReadClip("Attack");
}

ModelAnimatorRenderScene::~ModelAnimatorRenderScene()
{
	delete modelAnimator;
}

void ModelAnimatorRenderScene::Update()
{
	modelAnimator->Update();
}

void ModelAnimatorRenderScene::PreRender()
{
}

void ModelAnimatorRenderScene::Render()
{
	modelAnimator->Render();
}

void ModelAnimatorRenderScene::PostRender()
{
}

void ModelAnimatorRenderScene::GUIRender()
{
}
