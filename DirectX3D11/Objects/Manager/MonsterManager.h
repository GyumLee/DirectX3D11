#pragma once

class MonsterManager : public Singleton<MonsterManager>
{
private:
	friend class Singleton;

	vector<Ninja*> monsters;

	Terrain* terrain;

	MonsterManager();
	~MonsterManager();

public:
	void Update();
	void Render();
	void PostRender();

	void CreateMonsters(Terrain* terrain, UINT poolCount = 3);

	Transform* RayCollision(Ray ray, Vector3* hitPoint);
};