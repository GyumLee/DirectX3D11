#include "Framework.h"
#include "BillboardScene.h"

BillboardScene::BillboardScene()
{
	//for (UINT i = 0; i < 100; i++)
	//{
	//	Quad* quad = new Quad();
	//	quad->GetMaterial()->SetDiffuseMap("Textures/UI/Crosshair.png");
	//	//quad->GetMaterial()->SetDiffuseMap("Textures/Landscape/tree.png");
	//	quad->scale *= 5;
	//	
	//	quad->position.x = Random(-10, 10);
	//	quad->position.z = Random(-10, 10);
	//	quad->position.y = 10;

	//	quads.push_back(quad);
	//}

	material = new Material();
	material->SetShader(L"Geometry/Billboard.hlsl");
	material->SetDiffuseMap("Textures/UI/Crosshair.png");

	geometryShader = Shader::AddGS(L"Geometry/Billboard.hlsl");

	VertexSize* vertices = new VertexSize[1000];

	for (UINT i = 0; i < 1000; i++)
	{
		vertices[i].position.x = Random(-10, 10);
		vertices[i].position.z = Random(-10, 10);
		vertices[i].position.y = 10;

		vertices[i].size.x = Random(5.0f, 10.0f);
		vertices[i].size.y = Random(5.0f, 10.0f);
	}

	vertexBuffer = new VertexBuffer(vertices, sizeof(VertexSize), 1000);
	delete[] vertices;

	blendState = new BlendState();
	//blendState->AlphaToCoverage(true);
	blendState->Alpha(true);

	depthStencil = new DepthStencilState();
	depthStencil->DepthWriteMask(D3D11_DEPTH_WRITE_MASK_ZERO);
}

BillboardScene::~BillboardScene()
{
	/*for (Quad* quad : quads)
		delete quad;*/

	delete material;
	delete vertexBuffer;

	delete blendState;
	delete depthStencil;
}

void BillboardScene::Update()
{
	//Vector3 temp = quad->position - CAM->position;
	//quad->rotation.y = atan2(temp.x, temp.z);

	/*for (Quad* quad : quads)
	{
		quad->rotation.x = CAM->rotation.x;
		quad->rotation.y = CAM->rotation.y;

		quad->UpdateWorld();
	}*/
}

void BillboardScene::PreRender()
{
}

void BillboardScene::Render()
{
	blendState->SetState();
	depthStencil->SetState();

	/*for (Quad* quad : quads)
		quad->Render();*/
	vertexBuffer->IASet(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	material->Set();
	geometryShader->Set();

	DC->Draw(1000, 0);

	DC->GSSetShader(nullptr, nullptr, 0);
}

void BillboardScene::PostRender()
{
}

void BillboardScene::GUIRender()
{
}
