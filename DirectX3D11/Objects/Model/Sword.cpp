#include "Framework.h"

Sword::Sword() : Model("Sword")
{
	Load();

	collider = new BoxCollider();
	collider->SetParent(this);
	collider->tag = "SwordCollider";
	collider->Load();
}

Sword::~Sword()
{
	delete collider;
}

void Sword::Update()
{
	if (!isActive) return;

	UpdateWorld();
	collider->UpdateWorld();
}

void Sword::Render()
{
	if (!isActive) return;

	Model::Render();
	collider->Render();
}

void Sword::GUIRender()
{
	Model::GUIRender();
	collider->GUIRender();
}
