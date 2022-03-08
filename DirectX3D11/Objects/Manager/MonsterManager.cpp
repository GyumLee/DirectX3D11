#include "Framework.h"

MonsterManager::MonsterManager()
{
	ninjaInstancing = new ModelAnimatorInstancing("Ninja");
	ninjaInstancing->ReadClip("Idle");
	ninjaInstancing->ReadClip("Run");
	ninjaInstancing->ReadClip("Attack");
	ninjaInstancing->ReadClip("Hit", 0, true);
	ninjaInstancing->ReadClip("Dying");

	kunaiInstancing = new ModelInstancing("Kunai");
}

MonsterManager::~MonsterManager()
{
	for (Ninja* monster : monsters)
		delete monster;
	
	delete ninjaInstancing;
	delete kunaiInstancing;
}

void MonsterManager::Update()
{
	ninjaInstancing->Update();
	kunaiInstancing->Update();

	for (Ninja* monster : monsters)
	{
		if (!monster->IsActive()) continue;

		monster->Update();

		if (BulletManager::Get()->Collision(monster->GetCollider()))
			monster->Hit();

		if (monster->GetCollider()->Collision(player->GetSwordCollider()))
			monster->Hit();
	}
}

void MonsterManager::Render()
{
	ninjaInstancing->Render();
	kunaiInstancing->Render();

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
	this->terrain = terrain;

	for (UINT i = 0; i < poolCount; i++)
	{
		Vector3 pos;
		pos.x = Random(0.0f, terrain->GetSize().x);
		pos.z = Random(0.0f, terrain->GetSize().y);

		Ninja* monster = new Ninja(i);
		monster->SetInstancing(ninjaInstancing);
		monster->SetTerrain(terrain);

		Transform* ninja = ninjaInstancing->Add();
		ninja->tag = "Monster_" + to_string(i);
		ninja->position = pos;
		ninja->scale *= 0.03f;

		monster->SetTransform(ninja);
		monster->SetEvent();

		Transform* kunai = kunaiInstancing->Add();
		monster->SetKunai(kunai);

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
			return monster->GetTransform();
		}
	}

	return nullptr;
}
