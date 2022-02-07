#pragma once

class ShootingScene : public Scene
{
private:
	Terrain* terrain;

	Hinata* hinata;

public:
	ShootingScene();
	~ShootingScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};