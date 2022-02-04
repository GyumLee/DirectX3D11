#include "Framework.h"

Model::Model(string name)
{
	tag = name;

	reader = new ModelReader(name);
	reader->SetShader(L"Basic/NormalMapping.hlsl");

	worldBuffer = new WorldBuffer();
}

Model::~Model()
{
	delete reader;
	delete worldBuffer;
}

void Model::Render()
{
	worldBuffer->Set(world);
	worldBuffer->SetVSBuffer(0);

	reader->Render();
}
