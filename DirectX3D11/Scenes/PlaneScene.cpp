#include "Framework.h"
#include "PlaneScene.h"

PlaneScene::PlaneScene()
	: width(10), height(10)
{
	material = new Material();
	material->SetShader(L"Texture.hlsl");
	material->SetDiffuseMap("Landscape/Tree.png");

	CreateMesh();

	worldBuffer = new WorldBuffer();

	rasterizerState[0] = new RasterizerState();
	rasterizerState[1] = new RasterizerState();
	rasterizerState[1]->FillMode(D3D11_FILL_WIREFRAME);
}

PlaneScene::~PlaneScene()
{
	delete material;
	delete mesh;

	delete worldBuffer;

	delete rasterizerState[0];
	delete rasterizerState[1];
}

void PlaneScene::Update()
{
}

void PlaneScene::PreRender()
{
}

void PlaneScene::Render()
{
	worldBuffer->SetVSBuffer(0);

	mesh->IASet();
	material->Set();

	rasterizerState[1]->SetState();

	DC->DrawIndexed(indices.size(), 0, 0);

	rasterizerState[0]->SetState();
}

void PlaneScene::PostRender()
{
}

void PlaneScene::GUIRender()
{
}

void PlaneScene::CreateMesh()
{
	//Vertices
	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			VertexType vertex;
			vertex.position = { (float)x, 0.0f, height - (float)z - 1 };
			vertex.uv.x = x / (float)(width - 1);
			vertex.uv.y = z / (float)(height - 1);
			//vertex.uv.y = 1.0f - z / (float)(height - 1);

			vertices.push_back(vertex);
		}
	}

	//Indices
	for (UINT z = 0; z < height - 1; z++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			/*
			1 2
			0 3
			*/
			indices.push_back(width * z + x);//0
			indices.push_back(width * (z + 1) + x + 1);//2
			indices.push_back(width * (z + 1) + x);//1

			indices.push_back(width * z + x);//0
			indices.push_back(width * z + x + 1);//3
			indices.push_back(width * (z + 1) + x + 1);//2
		}
	}

	mesh = new Mesh(vertices.data(), sizeof(VertexType), vertices.size(),
		indices.data(), indices.size());
}
