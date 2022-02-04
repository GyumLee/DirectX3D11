#include "Framework.h"

Ninja::Ninja()
	: ModelAnimator("Ninja")
{
	ReadClip("Idle");
	ReadClip("Run");
	ReadClip("Attack");

	kunai = new Model("Kunai");
	kunai->SetParent(&rightHand);
	kunai->Load();
}

Ninja::~Ninja()
{
	delete kunai;
}

void Ninja::Update()
{
	SetRightHand();

	kunai->UpdateWorld();

	ModelAnimator::Update();
}

void Ninja::Render()
{
	ModelAnimator::Render();

	kunai->Render();
}

void Ninja::GUIRender()
{
	ModelAnimator::GUIRender();

	kunai->GUIRender();
}

void Ninja::SetRightHand()
{
	rightHand = GetTransformByNode(11) * world;
}
