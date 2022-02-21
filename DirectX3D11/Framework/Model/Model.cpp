#include "Framework.h"

Model::Model(string name)
{
	tag = name;

	reader = new ModelReader(name);
	reader->SetShader(L"Lighting/ModelLighting.hlsl");

	worldBuffer = new WorldBuffer();
}

Model::~Model()
{
	delete reader;
	delete worldBuffer;
}

void Model::Render()
{
	if (!isActive) return;

	worldBuffer->Set(world);
	worldBuffer->SetVSBuffer(0);

	reader->Render();
}
