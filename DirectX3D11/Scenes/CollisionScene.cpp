#include "Framework.h"
#include "CollisionScene.h"

CollisionScene::CollisionScene()
{
	Collider* collider = new CapsuleCollider(1, 5);
	collider->tag = "Capsule1";
	colliders.push_back(collider);

	collider = new CapsuleCollider(1, 3);
	collider->tag = "Capsule2";
	colliders.push_back(collider);
}

CollisionScene::~CollisionScene()
{
	for (Collider* collider : colliders)
		delete collider;
}

void CollisionScene::Update()
{
	if (colliders[0]->Collision(colliders[1]))
	{
		colliders[0]->SetColor(1, 0, 0);
		colliders[1]->SetColor(1, 0, 0);
	}
	else
	{
		colliders[0]->SetColor(0, 1, 0);
		colliders[1]->SetColor(0, 1, 0);
	}

	/*Ray ray = CAM->ScreenPointToRay(mousePos);

	Contact contact;

	if (colliders[0]->RayCollision(ray, &contact))
	{
		colliders[0]->SetColor(1, 0, 0);
		colliders[1]->position = contact.hitPoint;
	}
	else
	{
		colliders[0]->SetColor(0, 1, 0);
	}*/

	for (Collider* collider : colliders)
		collider->UpdateWorld();
}

void CollisionScene::PreRender()
{
}

void CollisionScene::Render()
{
	for (Collider* collider : colliders)
		collider->Render();
}

void CollisionScene::PostRender()
{
}

void CollisionScene::GUIRender()
{
	for (Collider* collider : colliders)
		collider->GUIRender();
}
