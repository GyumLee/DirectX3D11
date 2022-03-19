#pragma once

class Water : public Transform
{
protected:
	typedef VertexUV VertexType;

	Material* material;
	Mesh* mesh;

	vector<VertexType> vertices;
	vector<UINT> indices;

	WorldBuffer* worldBuffer;
	
	Reflection* reflection;
	Refraction* refraction;

	BlendState* blendState[2];

	Float2 size;

public:
	Water(string normalFile, float width = 100.0f, float height = 100.0f);
	~Water();

	void Update();

	void SetReflection();
	void SetRefraction();

	void Render();

	void GUIRender();

private:
	void CreateMesh();
};