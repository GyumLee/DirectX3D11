#include "Framework.h"

MonsterManager::MonsterManager()
{
}

MonsterManager::~MonsterManager()
{
	for (Ninja* monster : monsters)
		delete monster;
}

void MonsterManager::Update()
{
	for (Ninja* monster : monsters)
		monster->Update();
}

void MonsterManager::Render()
{
	for (Ninja* monster : monsters)
		monster->Render();
}

void MonsterManager::PostRender()
{
	for (Ninja* monster : monsters)
		monster->PostRender();
}

void MonsterManager::CreateMonsters(Terrain* terrain, UINT poolCount)
{
	for (UINT i = 0; i < poolCount; i++)
	{
		Vector3 pos;
		pos.x = Random(0.0f, terrain->GetSize().x);
		pos.z = Random(0.0f, terrain->GetSize().y);

		Ninja* monster = new Ninja();
		monster->tag = "Monster_" + to_string(i);
		monster->position = pos;
		monster->SetTerrain(terrain);

		monsters.push_back(monster);
	}
}

Transform* MonsterManager::RayCollision(Ray ray, Vector3* hitPoint)
{
	Contact contact;
	for (Ninja* monster : monsters)
	{
		if (monster->GetCollider()->RayCollision(ray, &contact))
		{
			*hitPoint = contact.hitPoint;
			return monster;
		}
	}

	return nullptr;
}
