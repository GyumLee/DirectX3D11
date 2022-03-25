#pragma once

class TerrainLOD : public Transform
{
private:
	typedef VertexUV VertexType;
	ValueBuffer* terrainBuffer;
	ValueBuffer* heightBuffer;
	WorldBuffer* worldBuffer;

	Material* material;
	Mesh* mesh;

	HullShader* hullShader;
	DomainShader* domainShader;

	UINT cellsPerPatch;
	float cellSpacing;
	Float2 cellUV;

	UINT width, height;
	UINT patchWidth, patchHeight;

	vector<VertexType> vertices;
	vector<UINT> indices;

	Texture* heightMap;

public:
	TerrainLOD(string heightFile);
	~TerrainLOD();

	void Update();
	void Render();
	void GUIRender();

private:
	void CreatePatchVertex();
	void CreatePatchIndex();
};