#pragma once

class Terrain : public Transform
{
private:
	const float MAX_HEIGHT = 30.0f;

	typedef VertexUVNormalTangent VertexType;

	Material* material;
	Mesh* mesh;

	vector<VertexType> vertices;
	vector<UINT> indices;

	UINT width, height;

	WorldBuffer* worldBuffer;

	RasterizerState* rasterizerState[2];

	Texture* heightMap;
	Texture* alphaMap;
	Texture* secondMap;
	Texture* thirdMap;

public:
	Terrain();
	~Terrain();

	void Render();

	float GetHeight(Vector3 pos);
	Vector3 Picking();

	Material* GetMaterial() { return material; }

	Float2 GetSize() { return Float2(width, height); }

private:
	void CreateMesh();
	void CreateNormal();
	void CreateTangent();
};