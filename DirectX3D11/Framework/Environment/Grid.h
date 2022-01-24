#pragma once

class Grid : public Transform
{
private:
	const int MAX_VALUE = 1000;

	typedef VertexColor VertexType;

	Material* material;
	VertexBuffer* vertexBuffer;

	vector<VertexType> vertices;

	UINT width, height;

	WorldBuffer* worldBuffer;

public:
	Grid();
	~Grid();

	void Render();
	void GUIRender();

private:
	void CreateMesh();
};