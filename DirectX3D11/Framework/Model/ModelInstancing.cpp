#include "Framework.h"

ModelInstancing::ModelInstancing(string name)
{
	reader = new ModelReader(name);
	reader->SetShader(L"Model/ModelInstancing.hlsl");

	size = reader->GetSize();

	instanceBuffer = new VertexBuffer(instanceDatas, sizeof(InstanceData), MAX_INSTANCE);
}

ModelInstancing::~ModelInstancing()
{
	for (Transform* transform : transforms)
		delete transform;

	delete instanceBuffer;
	delete reader;
}

void ModelInstancing::Update()
{
	drawCount = 0;

	for (Transform* transform : transforms)
	{
		if (!transform->isActive) continue;

		transform->UpdateWorld();

		if (!FRUSTUM->ContainBox(transform->GlobalPos(), size * transform->GlobalScale()))
			continue;

		instanceDatas[drawCount].transform = XMMatrixTranspose(transform->GetWorld());
		drawCount++;
	}

	instanceBuffer->Update(instanceDatas, drawCount);
}

void ModelInstancing::Render()
{
	instanceBuffer->IASet(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 1);

	reader->Render(drawCount);
}

void ModelInstancing::GUIRender()
{
	ImGui::Text("DrawCount : %d", drawCount);

	for (Transform* transform : transforms)
		transform->GUIRender();
}

Transform* ModelInstancing::Add()
{
	Transform* transform = new Transform();
	transform->tag = reader->GetName() + "_" + to_string(transforms.size());
	transforms.push_back(transform);

	return transform;
}
