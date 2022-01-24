#pragma once

class PlaneScene : public Scene
{
private:
	typedef VertexUV VertexType;

	Material* material;
	Mesh* mesh;

	vector<VertexType> vertices;
	vector<UINT> indices;

	UINT width, height;

	WorldBuffer* worldBuffer;

	RasterizerState* rasterizerState[2];

public:
	PlaneScene();
	~PlaneScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	void CreateMesh();
};