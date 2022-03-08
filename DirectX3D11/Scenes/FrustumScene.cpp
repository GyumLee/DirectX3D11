#include "Framework.h"
#include "FrustumScene.h"

FrustumScene::FrustumScene()
{
	for (float y = -10.0f; y < 10.0f; y++)
	{
		for (float x = -10.0f; x < 10.0f; x++)
		{
			Sphere* sphere = new Sphere(0.5f);
			sphere->position = { x, y, 0.0f };
			sphere->UpdateWorld();

			spheres.push_back(sphere);
		}
	}

	//frustum = new Frustum(Environment::Get()->GetProjection());
}

FrustumScene::~FrustumScene()
{
	for (Sphere* sphere : spheres)
		delete sphere;

	//delete frustum;
}

void FrustumScene::Update()
{
	//frustum->Update();
}

void FrustumScene::PreRender()
{
}

void FrustumScene::Render()
{
	drawCount = 0;

	for (Sphere* sphere : spheres)
	{
		//if (frustum->ContainPoint(sphere->position))
		if (FRUSTUM->ContainSphere(sphere->position, sphere->Radius()))
		{
			sphere->Render();
			drawCount++;
		}
	}
}

void FrustumScene::PostRender()
{
}

void FrustumScene::GUIRender()
{
	ImGui::Text("DrawCount : %d", drawCount);
}
