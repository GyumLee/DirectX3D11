#pragma once

class Cube : public Transform
{
private:
	typedef VertexUVNormal VertexType;

	Material* material;
	Mesh* mesh;

	vector<VertexType> vertices;
	vector<UINT> indices;

	WorldBuffer* worldBuffer;

	Float3 size;

public:
	Cube(Float3 size = Float3(1, 1, 1));
	~Cube();

	void Render();

	Material* GetMaterial() { return material; }

private:
	void CreateMesh();
	void CreateSmoothMesh(); //CreateSmoothEdge
	void CreateNormal();
};