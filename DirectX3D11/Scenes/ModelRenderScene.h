#pragma once

class ModelRenderScene : public Scene
{
private:
	Model* model;
	BoxCollider* boxCollider;

	BlendState* blendState[2];
	//RasterizerState* rasterizerState[2];

public:
	ModelRenderScene();
	~ModelRenderScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};