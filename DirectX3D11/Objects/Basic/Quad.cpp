#include "Framework.h"

Quad::Quad(float width, float height)
	: size(width, height)
{
	material = new Material();
	material->SetShader(L"Basic/Texture.hlsl");

	CreateMesh();

	worldBuffer = new WorldBuffer();
}

Quad::~Quad()
{
	delete material;
	delete mesh;
	delete worldBuffer;
}

void Quad::Render()
{
	worldBuffer->Set(world);
	worldBuffer->SetVSBuffer(0);

	mesh->IASet();
	material->Set();

	DC->DrawIndexed(indices.size(), 0, 0);
}

void Quad::CreateMesh()
{
	float halfX = size.x * 0.5f;
	float halfY = size.y * 0.5f;
	vertices.resize(4);
	vertices[0].position = { -halfX, +halfY, 0.0f };
	vertices[1].position = { +halfX, +halfY, 0.0f };
	vertices[2].position = { -halfX, -halfY, 0.0f };
	vertices[3].position = { +halfX, -halfY, 0.0f };

	vertices[0].uv = { 0, 0 };
	vertices[1].uv = { 1, 0 };
	vertices[2].uv = { 0, 1 };
	vertices[3].uv = { 1, 1 };
	/*
	0 1
	2 3
	*/
	indices = { 0, 1, 2, 2, 1, 3 };

	mesh = new Mesh(vertices.data(), sizeof(VertexType), vertices.size(),
		indices.data(), indices.size());
}
