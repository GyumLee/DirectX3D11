#include "Framework.h"
#include "ModelAnimatorInstancingScene.h"

ModelAnimatorInstancingScene::ModelAnimatorInstancingScene()
{
	modelAnimatorInstancing = new ModelAnimatorInstancing("Hinata");

	modelAnimatorInstancing->ReadClip("Idle");
	modelAnimatorInstancing->ReadClip("Run");
	modelAnimatorInstancing->ReadClip("Fire");
	modelAnimatorInstancing->ReadClip("SwordIdle");
	modelAnimatorInstancing->ReadClip("SwordRun");
	modelAnimatorInstancing->ReadClip("SwordSlash");
	modelAnimatorInstancing->ReadClip("PutBackRifle");
	modelAnimatorInstancing->ReadClip("GrabRifle");
	modelAnimatorInstancing->ReadClip("DrawSword");
	modelAnimatorInstancing->ReadClip("SheathSword");

	float interval = 20.0f;

	for (float z = -5.0f; z < 5.0f; z++)
	{
		for (float x = -5.0f; x < 5.0f; x++)
		{
			Transform* transform = modelAnimatorInstancing->Add();
			transform->position = { x, 0, z };
			transform->position *= interval;
			transform->scale *= 0.1f;
		}
	}

	for (int i = 0; i < MAX_INSTANCE; i++)
	{
		modelAnimatorInstancing->AddEvent(i, 2, 0.8f, bind(&ModelAnimatorInstancingScene::SetIdle, this));
	}
}

ModelAnimatorInstancingScene::~ModelAnimatorInstancingScene()
{
	delete modelAnimatorInstancing;
}

void ModelAnimatorInstancingScene::Update()
{
	modelAnimatorInstancing->Update();
}

void ModelAnimatorInstancingScene::PreRender()
{
}

void ModelAnimatorInstancingScene::Render()
{
	modelAnimatorInstancing->Render();
}

void ModelAnimatorInstancingScene::PostRender()
{
}

void ModelAnimatorInstancingScene::GUIRender()
{
	ImGui::SliderInt("InstanceID", &instanceID, 0, MAX_INSTANCE - 1);
	ImGui::SliderInt("SelectClip", &selectClip, 0, 9);
	if (ImGui::Button("Play"))
	{
		modelAnimatorInstancing->PlayClip(instanceID, selectClip);
	}
}

void ModelAnimatorInstancingScene::SetIdle()
{
	modelAnimatorInstancing->PlayClip(instanceID, 0);
}
