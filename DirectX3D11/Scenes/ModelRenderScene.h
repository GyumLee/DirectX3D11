#pragma once

class ModelRenderScene : public Scene
{
private:
	ModelBone* model;

public:
	ModelRenderScene();
	~ModelRenderScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};