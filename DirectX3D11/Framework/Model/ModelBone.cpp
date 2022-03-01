#include "Framework.h"

ModelBone::ModelBone(string name)
{
	tag = name;

	reader = new ModelReader(name);
	reader->SetShader(L"Lighting/BoneLighting.hlsl");

	worldBuffer = new WorldBuffer();
	boneBuffer = new BoneBuffer();

	MakeTransform();
}

ModelBone::~ModelBone()
{
	delete worldBuffer;
	delete boneBuffer;

	delete reader;
}

void ModelBone::Render()
{
	worldBuffer->Set(world);
	worldBuffer->SetVSBuffer(0);
	boneBuffer->SetVSBuffer(10);

	reader->Render();
}

void ModelBone::MakeTransform()
{
	vector<NodeData> nodes = reader->GetNodes();

	UINT nodeIndex = 0;
	for (NodeData node : nodes)
	{
		//Matrix parent;
		boneBuffer->data.bones[nodeIndex] = XMMatrixTranspose(node.transform);
		nodeIndex++;
	}
}
