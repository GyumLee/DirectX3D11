#pragma once

class TerrainData
{
private:
	const float MAX_HEIGHT = 30.0f;

	typedef VertexUVNormal VertexType;

	vector<VertexType> vertices;
	vector<VertexType> prevData;
	vector<UINT> indices;

	UINT width, height;

	Texture* heightMap;

public:
	TerrainData(string heightFile);
	~TerrainData();

	vector<VertexType> GetVertices() { return vertices; }

	Float2 GetSize() { return Float2(width, height); }

private:
	void CreateMesh();
	void CreateNormal();
	void AlignVertexData();
};