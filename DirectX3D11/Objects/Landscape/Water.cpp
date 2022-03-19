#include "Framework.h"

Water::Water(string normalFile, float width, float height)
	: size(width, height)
{
	material = new Material();
	material->SetShader(L"Water/Water.hlsl");

	worldBuffer = new WorldBuffer();

	reflection = new Reflection(this);
	refraction = new Refraction(normalFile);

	blendState[0] = new BlendState();
	blendState[1] = new BlendState();
	blendState[1]->Alpha(true);

	CreateMesh();
}

Water::~Water()
{
	delete material;
	delete mesh;
	delete worldBuffer;

	delete reflection;
	delete refraction;

	delete blendState[0];
	delete blendState[1];
}

void Water::Update()
{
	reflection->Update();
	refraction->Update();

	UpdateWorld();
}

void Water::SetReflection()
{
	reflection->SetReflection();
}

void Water::SetRefraction()
{
	refraction->SetRefraction();
}

void Water::Render()
{
	worldBuffer->Set(world);
	worldBuffer->SetVSBuffer(0);

	reflection->SetRender();
	refraction->SetRender();

	mesh->IASet();
	material->Set();

	blendState[1]->SetState();
	DC->DrawIndexed(indices.size(), 0, 0);
	blendState[0]->SetState();
}

void Water::GUIRender()
{
	Transform::GUIRender();
	refraction->GUIRender();
}

void Water::CreateMesh()
{
	float halfX = size.x * 0.5f;
	float halfZ = size.y * 0.5f;

	vertices.resize(4);
	vertices[0].position = { -halfX, 0.0f, -halfZ };
	vertices[1].position = { -halfX, 0.0f, +halfZ };
	vertices[2].position = { +halfX, 0.0f, -halfZ };
	vertices[3].position = { +halfX, 0.0f, +halfZ };

	vertices[0].uv = { 0, 1 };
	vertices[1].uv = { 0, 0 };
	vertices[2].uv = { 1, 1 };
	vertices[3].uv = { 1, 0 };

	indices = { 0, 1, 2, 2, 1, 3 };

	mesh = new Mesh(vertices.data(), sizeof(VertexType), vertices.size(),
		indices.data(), indices.size());
}
