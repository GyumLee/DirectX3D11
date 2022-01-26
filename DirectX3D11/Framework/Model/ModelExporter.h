#pragma once

class ModelExporter
{
private:
	Assimp::Importer* importer;
	const aiScene* scene;

	string name;

	vector<Material*> materials;
	vector<MeshData*> meshes;
	vector<NodeData*> nodes;
	vector<BoneData*> bones;

	map<string, UINT> boneMap;
	UINT boneCount = 0;

public:
	ModelExporter(string name, string file);
	~ModelExporter();

	void ExportMaterial();
	void ExportMesh();
	
private:
	//Material
	void ReadMaterial();
	void WriteMaterial();
	string CreateTexture(string file);

	//Mesh
	void ReadMesh(aiNode* node);
	void ReadNode(aiNode* node, int index, int parent);
	void ReadBone(aiMesh* mesh, vector<VertexWeights>& vertexWeights);
	void WriteMesh();
};