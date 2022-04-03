#include "Framework.h"

Sword::Sword() : Model("Sword")
{
	Load();

	collider = new BoxCollider();
	collider->SetParent(this);
	collider->tag = "SwordCollider";
	collider->Load();

	start.tag = "SwordTrailStart";
	end.tag = "SwordTrailEnd";

	start.SetParent(this);
	end.SetParent(this);

	start.Load();
	end.Load();
	start.position.y = 10;

	trail = new Trail("Textures/UI/Crosshair.png", &startPos, &endPos, 3);
	trail->isActive = false;

	//Material* material = reader->GetMaterial(0);
	//material->SetShader(L"Basic/Texture.hlsl");
}

Sword::~Sword()
{
	delete collider;
}

void Sword::Update()
{
	if (!isActive) return;

	start.UpdateWorld();
	end.UpdateWorld();

	startPos = start.GlobalPos();
	endPos = end.GlobalPos();

	trail->Update();

	UpdateWorld();
	collider->UpdateWorld();
}

void Sword::Render()
{
	if (!isActive) return;

	Model::Render();
	collider->Render();

	trail->Render();
}

void Sword::GUIRender()
{
	Model::GUIRender();
	collider->GUIRender();

	start.GUIRender();
	end.GUIRender();
}
