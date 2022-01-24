#pragma once

class TerrainScene : public Scene
{
private:
	Terrain* terrain;

	Sphere* sphere;

	Vector3 destPos;

public:
	TerrainScene();
	~TerrainScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};