#include "Framework.h"

ModelBone::ModelBone(string name)
{
	tag = name;

	reader = new ModelReader(name);
	reader->SetShader(L"ModelBone.hlsl");

	worldBuffer = new WorldBuffer();
	boneBuffer = new BoneBuffer();

	MakeTransform();
}

ModelBone::~ModelBone()
{
	delete worldBuffer;
	delete boneBuffer;

	delete reader;

	delete[] nodeTransforms;
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

	nodeTransforms = new Matrix[nodes.size()];

	UINT nodeIndex = 0;
	for (NodeData node : nodes)
	{
		//Matrix parent;
		boneBuffer->data.bones[nodeIndex] = XMMatrixTranspose(node.transform);
		nodeIndex++;
	}
}
