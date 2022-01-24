#pragma once

class GridScene : public Scene
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
	GridScene();
	~GridScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	void CreateMesh();
};