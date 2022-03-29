#include "Framework.h"
#include "ModelInstancingScene.h"

ModelInstancingScene::ModelInstancingScene()
{
	modelInstancing = new ModelInstancing("Sword");

	float interval = 20.0f;
	float instanceCount = 0;
	for (float z = -10.0f; z < 10.0f; z++)
	{
		if (instanceCount >= MAX_INSTANCE) break;
		for (float x = -10.0f; x < 10.0f; x++)
		{
			Transform* transform = modelInstancing->Add();
			transform->position = { x, 0, z };
			transform->position *= interval;

			/*Model* model = new Model("Sword");
			model->position = { x, 0, z };
			model->position *= interval;
			model->UpdateWorld();
			models.push_back(model);*/

			instanceCount++;
			if (instanceCount >= MAX_INSTANCE) break;
		}
	}
}

ModelInstancingScene::~ModelInstancingScene()
{
	/*for (Model* model : models)
		delete model;*/
	delete modelInstancing;
}

void ModelInstancingScene::Update()
{
	modelInstancing->Update();
}

void ModelInstancingScene::PreRender()
{
}

void ModelInstancingScene::Render()
{
	/*for (Model* model : models)
		model->Render();*/
	modelInstancing->Render();
}

void ModelInstancingScene::PostRender()
{
}

void ModelInstancingScene::GUIRender()
{
	modelInstancing->GUIRender();
}
