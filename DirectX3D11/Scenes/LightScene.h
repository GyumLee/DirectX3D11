#pragma once

class LightScene : public Scene
{
private:
	Quad* quad;

	Model* sword;
	ModelAnimator* hinata;
	Sphere* sphere;

public:
	LightScene();
	~LightScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	void Create();
};