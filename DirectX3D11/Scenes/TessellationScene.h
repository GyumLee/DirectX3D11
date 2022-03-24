#pragma once

class TessellationScene : public Scene
{
private:
	static const int VERTEX_COUNT = 4;

	Material* material;
	HullShader* hullShader;
	DomainShader* domainShader;

	VertexBuffer* vertexBuffer;

	RasterizerState* rsState;

	ValueBuffer* edgeBuffer;
	ValueBuffer* insideBuffer;

public:
	TessellationScene();
	~TessellationScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};