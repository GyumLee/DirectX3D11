#include "Framework.h"

Hinata::Hinata()
	: ModelAnimator("Hinata"), state(IDLE)
{
	Load();

	SetMotions();

	rifle = new Model("Rifle");
	rifle->SetParent(&rightHand);
	rifle->Load();
}

Hinata::~Hinata()
{
	delete rifle;
}

void Hinata::Update()
{
	rightHand = GetTransformByNode(204) * world;

	Move();
	Fire();

	ModelAnimator::Update();
	rifle->UpdateWorld();
}

void Hinata::Render()
{
	ModelAnimator::Render();
	rifle->Render();
}

void Hinata::GUIRender()
{
	Transform::GUIRender();
	rifle->GUIRender();
}

void Hinata::Move()
{
	if (isFire) return;

	if (KEY_PRESS('W'))
	{
		position -= Forward() * moveSpeed * DELTA;
		SetClip(RUN);
	}
	else if (KEY_PRESS('S'))
	{
		position += Forward() * moveSpeed * DELTA;
		SetClip(RUN);
	}

	ImVec2 delta = ImGui::GetIO().MouseDelta;

	rotation.y += delta.x * DELTA * rotSpeed;

	CAM->GetFocusOffset().y -= delta.y * DELTA * rotSpeed * 5.0f;

	/*if (KEY_PRESS('A'))
		rotation.y -= rotSpeed * DELTA;
	else if (KEY_PRESS('D'))
		rotation.y += rotSpeed * DELTA;*/

	if (KEY_UP('W') || KEY_UP('S'))
		SetClip(IDLE);

	if (terrain != nullptr)
		position.y = terrain->GetHeight(position);
}

void Hinata::Fire()
{
	if (isFire) return;

	if (MOUSE_CLICK(0))
	{
		isFire = true;
		SetClip(FIRE);
	}
}

void Hinata::EndFire()
{
	isFire = false;
	SetClip(IDLE);
}

void Hinata::SetClip(AnimState state)
{
	if (this->state != state)
	{
		this->state = state;
		PlayClip(state);
	}
}

void Hinata::SetMotions()
{
	ReadClip("Idle");
	ReadClip("Run");
	ReadClip("Fire");

	clips[FIRE]->SetEvent(0.9f, bind(&Hinata::EndFire, this));
}
