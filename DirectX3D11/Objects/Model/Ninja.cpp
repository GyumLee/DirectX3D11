#include "Framework.h"

Ninja::Ninja()
	: ModelAnimator("Ninja"), state(IDLE)
{
	SetMotions();

	kunai = new Model("Kunai");
	kunai->SetParent(&rightHand);
	kunai->Load();

	collider = new SphereCollider();
	collider->tag = "NinjaCollider";
	collider->SetParent(this);
	collider->Load();

	Load();

	hpBar = new ProgressBar("Textures/UI/hp_bar.png", "Textures/UI/hp_bar_BG.png");
	//hpBar->scale *= 0.1f;
}

Ninja::~Ninja()
{
	delete kunai;
	delete collider;
	delete hpBar;
}

void Ninja::Update()
{
	if (!isActive) return;

	SetRightHand();
	SetHpBar();

	//Trace();
	Move();

	kunai->UpdateWorld();

	ModelAnimator::Update();
	collider->UpdateWorld();
}

void Ninja::Render()
{
	if (!isActive) return;

	ModelAnimator::Render();

	kunai->Render();
	collider->Render();
}

void Ninja::PostRender()
{
	if (!isActive) return;

	hpBar->Render();
}

void Ninja::GUIRender()
{
	ModelAnimator::GUIRender();
	kunai->GUIRender();
	collider->GUIRender();
}

void Ninja::Move()
{
	if (terrain)
		position.y = terrain->GetHeight(position);
}

void Ninja::Hit()
{
	collider->isActive = false;

	hp -= 30.0f;

	hpBar->SetValue(hp);

	if (hp > 0)
		SetClip(HIT);
	else
		SetClip(DYING);
}

void Ninja::Trace()
{
	if (!collider->isActive) return;

	if (!target) return;

	Vector3 dir = target->position - position;

	Vector3 cross = Vector3::Cross(dir, Forward());

	if (cross.y > 0.01f)
		rotation.y += DELTA;
	else if (cross.y < -0.01f)
		rotation.y -= DELTA;

	position -= Forward() * DELTA;

	SetClip(RUN);
}

void Ninja::EndHit()
{
	SetClip(IDLE);
	collider->isActive = true;
}

void Ninja::EndDie()
{
	isActive = false;
}

void Ninja::SetHpBar()
{
	float distance = Distance(CAM->position, position);
	hpBar->scale.x = 10 / distance;
	hpBar->scale.y = 10 / distance;

	Vector3 barPos = position + Vector3(0, 5, 0);

	hpBar->position = CAM->WorldToScreenPoint(barPos);

	lerpHp = LERP(lerpHp, hp, lerpSpeed * DELTA);
	hpBar->SetLerpValue(lerpHp);
	hpBar->Update();
}

void Ninja::SetRightHand()
{
	rightHand = GetTransformByNode(11) * world;
}

void Ninja::SetMotions()
{
	ReadClip("Idle");
	ReadClip("Run");
	ReadClip("Attack");
	//ReadClip("Hit"); // 1. BreakPoint here, Start Debugging, go to ReadClip()
	ReadClip("Hit", 0, true);
	ReadClip("Dying");

	clips[HIT]->SetEvent(0.8f, bind(&Ninja::EndHit, this));
	clips[DYING]->SetEvent(0.9f, bind(&Ninja::EndDie, this));
}

void Ninja::SetClip(AnimState state)
{
	if (this->state != state)
	{
		this->state = state;
		PlayClip(state);
	}
}
