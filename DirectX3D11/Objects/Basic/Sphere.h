#pragma once

class Sphere : public Transform
{
private:
	typedef VertexUVNormalTangent VertexType;

	Material* material;
	Mesh* mesh;

	vector<VertexType> vertices;
	vector<UINT> indices;

	WorldBuffer* worldBuffer;

	float radius;
	UINT sliceCount;
	UINT stackCount;

public:
	Sphere(float radius = 1.0f, UINT sliceCount = 20, UINT stackCount = 10);
	~Sphere();

	void Render();

	Material* GetMaterial() { return material; }

private:
	void CreateMesh();
	void CreateTangent();
};