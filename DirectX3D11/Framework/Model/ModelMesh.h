#pragma once

class ModelMesh
{
private:
	string name;

	Material* material;
	Mesh* mesh;

	UINT indexCount;

	BoneIndexBuffer* boneIndexBuffer;

public:
	ModelMesh(string name);
	~ModelMesh();

	void Render();

	void CreateMesh(void* vertexData, UINT vertexCount, void* indexData, UINT indexCount);

	void SetMaterial(Material* material) { this->material = material; }

	void SetBoneIndex(int index) { boneIndexBuffer->data.index = index; }

	string GetName() { return name; }
};