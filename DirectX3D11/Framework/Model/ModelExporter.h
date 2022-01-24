#pragma once

class ModelExporter
{
private:
	Assimp::Importer* importer;
	const aiScene* scene;

	string name;

	vector<Material*> materials;

public:
	ModelExporter(string name, string file);
	~ModelExporter();

	void ExportMaterial();
	
private:
	void ReadMaterial();
	void WriteMaterial();
	string CreateTexture(string file);
};