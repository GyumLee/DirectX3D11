#include "Framework.h"
#include "ModelRenderScene.h"

ModelRenderScene::ModelRenderScene()
{
	string name = "Fence_MetalChainlink_2x";

	model = new Model(name);

	blendState[0] = new BlendState();
	blendState[1] = new BlendState();
	blendState[1]->Alpha(true);

	//rasterizerState[0] = new RasterizerState();
	//rasterizerState[1] = new RasterizerState();
	//rasterizerState[1]->FrontCounterClockwise(true);

	boxCollider = new BoxCollider(model->GetReader()->GetSize());
	boxCollider->SetParent(model);
}

ModelRenderScene::~ModelRenderScene()
{
	delete model;

	//delete rasterizerState[0];
	//delete rasterizerState[1];

	delete blendState[0];
	delete blendState[1];

	delete boxCollider;
}

void ModelRenderScene::Update()
{
	model->UpdateWorld();

	boxCollider->UpdateWorld();
}

void ModelRenderScene::PreRender()
{
}

void ModelRenderScene::Render()
{
	//rasterizerState[1]->SetState();
	blendState[1]->SetState();

	model->Render();

	blendState[0]->SetState();

	boxCollider->Render();

	//rasterizerState[0]->SetState();
}

void ModelRenderScene::PostRender()
{
}

void ModelRenderScene::GUIRender()
{
	model->GUIRender();
	model->GetReader()->GUIRender();
	boxCollider->GUIRender();
}
