#pragma once

class ModelAnimatorRenderScene : public Scene
{
private:
	ModelAnimator* modelAnimator;

public:
	ModelAnimatorRenderScene();
	~ModelAnimatorRenderScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};