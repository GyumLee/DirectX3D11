#include "Framework.h"

Obstacle::Obstacle(string name, Collider::Type type)
	: Model(name)
{
	Load();

	switch (type)
	{
	case Collider::BOX:
		collider = new BoxCollider();
		break;
	case Collider::SPHERE:
		collider = new SphereCollider();
		break;
	case Collider::CAPSULE:
		collider = new CapsuleCollider();
		break;
	}

	collider->SetParent(this);
	collider->tag = name + "Collider";
	collider->Load();
}

Obstacle::~Obstacle()
{
	delete collider;
}

void Obstacle::Update()
{
	UpdateWorld();
	collider->UpdateWorld();
}

void Obstacle::Render()
{
	Model::Render();
	collider->Render();
}

void Obstacle::GUIRender()
{
	Transform::GUIRender();
	collider->GUIRender();
}
