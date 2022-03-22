#pragma once

class ShadowScene : public Scene
{
private:
	SkyBox* skybox;

	Quad* quad;

	Model* sword;
	ModelAnimator* hinata;
	Sphere* sphere;

	Shadow* shadow;

public:
	ShadowScene();
	~ShadowScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	void Create();
};