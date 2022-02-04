#include "Framework.h"

bool CapsuleCollider::RayCollision(IN Ray ray, OUT Contact* contact)
{
	return false;
}

bool CapsuleCollider::BoxCollision(BoxCollider* collider)
{
	return false;
}

bool CapsuleCollider::SphereCollision(SphereCollider* collider)
{
	return false;
}

bool CapsuleCollider::CapsuleCollision(CapsuleCollider* collider)
{
	return false;
}

void CapsuleCollider::CreateMesh()
{
}
