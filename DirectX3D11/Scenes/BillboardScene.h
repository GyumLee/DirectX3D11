#pragma once

class BillboardScene : public Scene
{
private:
	//vector<Quad*> quads;

	Material* material;
	VertexBuffer* vertexBuffer;

	GeometryShader* geometryShader;

	BlendState* blendState;
	DepthStencilState* depthStencil;

public:
	BillboardScene();
	~BillboardScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};