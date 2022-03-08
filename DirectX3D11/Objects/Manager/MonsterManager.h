#pragma once

class MonsterManager : public Singleton<MonsterManager>
{
private:
	friend class Singleton;

	ModelAnimatorInstancing* ninjaInstancing;
	ModelInstancing* kunaiInstancing;

	vector<Ninja*> monsters;

	Terrain* terrain;

	Hinata* player;

	MonsterManager();
	~MonsterManager();

public:
	void Update();
	void Render();
	void PostRender();

	void CreateMonsters(Terrain* terrain, UINT poolCount = 3);

	Transform* RayCollision(Ray ray, Vector3* hitPoint);

	void SetPlayer(Hinata* hinata) { player = hinata; }
};