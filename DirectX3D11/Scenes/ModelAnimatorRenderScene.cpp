#include "Framework.h"
#include "ModelAnimatorRenderScene.h"

ModelAnimatorRenderScene::ModelAnimatorRenderScene()
{
	string name = "Character";

	modelAnimator = new ModelAnimator(name);

	modelAnimator->ReadClip("idle", 1);
	modelAnimator->ReadClip("run", 1);
	modelAnimator->ReadClip("jump", 1);

	modelAnimator->Load();
}

ModelAnimatorRenderScene::~ModelAnimatorRenderScene()
{
	delete modelAnimator;
}

void ModelAnimatorRenderScene::Update()
{
	if (KEY_DOWN('1'))
		modelAnimator->PlayClip(0);
	if (KEY_DOWN('2'))
		modelAnimator->PlayClip(1);
	if (KEY_DOWN('3'))
		modelAnimator->PlayClip(2);

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
	modelAnimator->GUIRender();
}
