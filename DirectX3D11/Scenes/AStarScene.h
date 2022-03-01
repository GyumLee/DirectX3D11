#pragma once

class AStarScene : public Scene
{
private:
	Terrain* terrain;
	Character* character;

	AStar* aStar;

	vector<Obstacle*> obstacles;

public:
	AStarScene();
	~AStarScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	void CreateObstacles();
};