#include "Framework.h"

bool BoxCollider::RayCollision(IN Ray ray, OUT Contact* contact)
{
	return false;
}

bool BoxCollider::BoxCollision(BoxCollider* collider)
{
	return false;
}

bool BoxCollider::SphereCollision(SphereCollider* collider)
{
	return false;
}

bool BoxCollider::CapsuleCollision(CapsuleCollider* collider)
{
	return false;
}

void BoxCollider::CreateMesh()
{
}
