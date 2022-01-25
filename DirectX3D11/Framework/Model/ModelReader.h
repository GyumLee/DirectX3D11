#pragma once

class ModelReader
{
private:
	string name;

	vector<Material*> materials;
	vector<ModelMesh*> meshes;

public:
	ModelReader(string name);
	~ModelReader();

	void Render();
	void GUIRender();

	void SetShader(wstring file);

private:
	void ReadMaterial();
	void ReadMesh();
};