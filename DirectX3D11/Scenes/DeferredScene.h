#pragma once

class DeferredScene : public Scene
{
private:
	Quad* quad;

	Model* sword;
	ModelAnimator* hinata;
	Sphere* sphere;

	GBuffer* gBuffer;

	Material* material;
	VertexBuffer* vertexBuffer;

public:
	DeferredScene();
	~DeferredScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	void Create();
};