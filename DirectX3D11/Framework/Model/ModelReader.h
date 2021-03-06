#pragma once

class ModelReader
{
private:
	string name;

	vector<Material*> materials;
	vector<ModelMesh*> meshes;
	vector<NodeData> nodes;
	vector<BoneData> bones;

	map<string, UINT> boneMap;

public:
	ModelReader(string name);
	~ModelReader();

	void Render();
	void Render(UINT instanceCount);

	void GUIRender();

	void SetShader(wstring file);

	int GetBone(string name);

	Vector3 GetSize();

	vector<NodeData>& GetNodes() { return nodes; }
	vector<BoneData>& GetBones() { return bones; }

	Material* GetMaterial(UINT index) { return materials[index]; }

	string GetName() { return name; }

private:
	void ReadMaterial();
	void ReadMesh();

	void SetMeshIndex(string name, int index);
};