#include "Framework.h"

Sword::Sword() : Model("Sword")
{
	Load();

	collider = new BoxCollider();
	collider->SetParent(this);
	collider->tag = "SwordCollider";
	collider->Load();

	Material* material = reader->GetMaterial(0);
	material->SetShader(L"Basic/Texture.hlsl");
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
