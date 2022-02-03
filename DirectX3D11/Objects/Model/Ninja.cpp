#include "Framework.h"

Ninja::Ninja()
	: ModelAnimator("Ninja")
{
	ReadClip("Idle");
	ReadClip("Run");
	ReadClip("Attack");

	sphere = new Sphere();
	sphere->SetParent(&rightHand);
}

Ninja::~Ninja()
{
	delete sphere;
}

void Ninja::Update()
{
	SetRightHand();

	sphere->UpdateWorld();

	ModelAnimator::Update();
}

void Ninja::Render()
{
	ModelAnimator::Render();

	sphere->Render();
}

void Ninja::GUIRender()
{
	ModelAnimator::GUIRender();

	sphere->GUIRender();
}

void Ninja::SetRightHand()
{
	rightHand = GetTransformByNode(35) * world;
}
