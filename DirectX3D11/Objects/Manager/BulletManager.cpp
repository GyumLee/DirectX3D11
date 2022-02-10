#include "Framework.h"

BulletManager::BulletManager()
{
}

BulletManager::~BulletManager()
{
	for (Bullet* bullet : bullets)
		delete bullet;
}

void BulletManager::Update()
{
	for (Bullet* bullet : bullets)
		bullet->Update();
}

void BulletManager::Render()
{
	for (Bullet* bullet : bullets)
		bullet->Render();
}

void BulletManager::CreateBullets(UINT poolCount)
{
	for (UINT i = 0; i < poolCount; i++)
	{
		Bullet* bullet = new Bullet();
		bullet->isActive = false;
		bullets.push_back(bullet);
	}
}

void BulletManager::Fire(Vector3 pos, Vector3 rot, float speed)
{
	for (Bullet* bullet : bullets)
	{
		if (!bullet->isActive)
		{
			bullet->Fire(pos, rot, speed);
			break;
		}
	}
}

bool BulletManager::Collision(Collider* collider)
{
	for (Bullet* bullet : bullets)
	{
		if (!bullet->isActive) continue;

		if (bullet->GetCollider()->Collision(collider))
		{
			bullet->isActive = false;
			return true;
		}
	}

	return false;
}
