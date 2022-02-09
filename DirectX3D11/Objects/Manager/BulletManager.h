#pragma once

class BulletManager : public Singleton<BulletManager>
{
private:
	friend class Singleton;

	vector<Bullet*> bullets;

	BulletManager();
	~BulletManager();

public:
	void Update();
	void Render();

	void CreateBullets(UINT poolCount = 30);

	void Fire(Vector3 pos, Vector3 rot, float speed = 10.0f);
};