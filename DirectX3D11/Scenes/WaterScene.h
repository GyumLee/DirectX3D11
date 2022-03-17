#pragma once

class WaterScene : public Scene
{
private:
	SkyBox* skybox;

	Quad* quad;

	Model* sword;
	ModelAnimator* hinata;
	Sphere* sphere;

	Reflection* reflection;

public:
	WaterScene();
	~WaterScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	void Create();
};