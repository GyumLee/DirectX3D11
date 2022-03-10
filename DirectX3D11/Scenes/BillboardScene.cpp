#include "Framework.h"
#include "BillboardScene.h"

BillboardScene::BillboardScene()
{
	for (UINT i = 0; i < 100; i++)
	{
		Quad* quad = new Quad();
		//quad->GetMaterial()->SetDiffuseMap("Textures/UI/Crosshair.png");
		quad->GetMaterial()->SetDiffuseMap("Textures/Landscape/tree.png");
		quad->scale *= 5;
		
		quad->position.x = Random(-10, 10);
		quad->position.z = Random(-10, 10);
		quad->position.y = 10;

		quads.push_back(quad);
	}

	blendState = new BlendState();
	//blendState->AlphaToCoverage(true);
	blendState->Alpha(true);

	depthStencil = new DepthStencilState();
	depthStencil->DepthWriteMask(D3D11_DEPTH_WRITE_MASK_ZERO);
}

BillboardScene::~BillboardScene()
{
	for (Quad* quad : quads)
		delete quad;

	delete blendState;
	delete depthStencil;
}

void BillboardScene::Update()
{
	//Vector3 temp = quad->position - CAM->position;
	//quad->rotation.y = atan2(temp.x, temp.z);

	for (Quad* quad : quads)
	{
		quad->rotation.x = CAM->rotation.x;
		quad->rotation.y = CAM->rotation.y;

		quad->UpdateWorld();
	}
}

void BillboardScene::PreRender()
{
}

void BillboardScene::Render()
{
	blendState->SetState();
	depthStencil->SetState();

	for (Quad* quad : quads)
		quad->Render();
}

void BillboardScene::PostRender()
{
}

void BillboardScene::GUIRender()
{
}
