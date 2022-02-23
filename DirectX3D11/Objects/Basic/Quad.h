#pragma once

class Quad : public Transform
{
private:
	typedef VertexUVNormalTangent VertexType;

	Material* material;
	Mesh* mesh;

	vector<VertexType> vertices;
	vector<UINT> indices;

	WorldBuffer* worldBuffer;

	Float2 size;

public:
	Quad(float width = 1.0f, float height = 1.0f);
	~Quad();

	void Render();
	
	Material* GetMaterial() { return material; }

private:
	void CreateMesh();
};