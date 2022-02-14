#pragma once

class ShootingScene : public Scene
{
private:
	Terrain* terrain;

	Hinata* hinata;

	Quad* cursor;

	SkyBox* skyBox;

public:
	ShootingScene();
	~ShootingScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};