#include "Framework.h"

RenderTransform::RenderTransform()
{
	material = new Material();
	material->SetShader(L"Basic/Grid.hlsl");

	CreateMesh();

	worldBuffer = new WorldBuffer();
}

RenderTransform::~RenderTransform()
{
	delete material;
	delete vertexBuffer;
	delete worldBuffer;
}

void RenderTransform::Render()
{
	if (!isActive) return;

	worldBuffer->Set(world);
	worldBuffer->SetVSBuffer(0);

	vertexBuffer->IASet(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	material->Set();

	DC->Draw(vertices.size(), 0);
}

void RenderTransform::CreateMesh()
{
	vertices.emplace_back(0, 0, 0, 1, 0, 0);
	vertices.emplace_back(1, 0, 0, 1, 0, 0);

	vertices.emplace_back(0, 0, 0, 0, 1, 0);
	vertices.emplace_back(0, 1, 0, 0, 1, 0);

	vertices.emplace_back(0, 0, 0, 0, 0, 1);
	vertices.emplace_back(0, 0, 1, 0, 0, 1);

	vertexBuffer = new VertexBuffer(vertices.data(),
		sizeof(VertexType), vertices.size());
}
