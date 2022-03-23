#pragma once

class QuadTreeTerrain : public Transform
{
private:
	typedef VertexUVNormal VertexType;

	const UINT MIN_TRIANGLE = 2048;

	struct Node
	{
		float x, z, size;
		UINT triangleCount;

		Mesh* mesh;

		Node* children[4];
	};

	TerrainData* terrainData;
	VertexType* vertices;

	Node* root;

	Material* material;
	WorldBuffer* worldBuffer;

	UINT width, height;
	UINT triangleCount, drawCount;

public:
	QuadTreeTerrain(string heightFile);
	~QuadTreeTerrain();

	void Render();
	void GUIRender();

private:
	void RenderNode(Node* node);
	void DeleteNode(Node* node);

	void CalcMeshDimensions(UINT vertexCount, float& centerX, float& centerZ, float& size);

	bool IsTriangleContained(UINT index, float x, float z, float size);
	UINT ContainTriangleCount(float x, float z, float size);

	void CreateTreeNode(Node* node, float x, float z, float size);
};