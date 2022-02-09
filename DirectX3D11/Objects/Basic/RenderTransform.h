#pragma once

class RenderTransform : public Transform
{
private:
	typedef VertexColor VertexType;

	Material* material;
	VertexBuffer* vertexBuffer;

	vector<VertexType> vertices;

	WorldBuffer* worldBuffer;

public:
	RenderTransform();
	~RenderTransform();

	void Render();

	Material* GetMaterial() { return material; }

private:
	void CreateMesh();
};