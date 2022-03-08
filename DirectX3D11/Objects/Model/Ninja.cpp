#include "Framework.h"

Ninja::Ninja(UINT instanceID)
	: instanceID(instanceID), state(IDLE)
{
	collider = new SphereCollider();
	collider->tag = "NinjaCollider";
	collider->Load();

	hpBar = new ProgressBar("Textures/UI/hp_bar.png", "Textures/UI/hp_bar_BG.png");
	//hpBar->scale *= 0.1f;
}

Ninja::~Ninja()
{
	delete collider;
	delete hpBar;
}

void Ninja::Update()
{
	SetLeftHand();
	SetHpBar();

	//Trace();
	Move();

	collider->UpdateWorld();
}

void Ninja::Render()
{
	if (!ninja->isActive) return;
	float radius = ((SphereCollider*)collider)->Radius();
	if (!FRUSTUM->ContainSphere(collider->GlobalPos(), radius)) return;

	collider->Render();
}

void Ninja::PostRender()
{
	if (!ninja->isActive) return;
	float radius = ((SphereCollider*)collider)->Radius();
	if (!FRUSTUM->ContainSphere(collider->GlobalPos(), radius)) return;

	hpBar->Render();
}

void Ninja::GUIRender()
{
	collider->GUIRender();
}

void Ninja::Move()
{
	if (terrain)
		ninja->position.y = terrain->GetHeight(ninja->position);
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

void Ninja::SetEvent()
{
	instancing->AddEvent(instanceID, HIT, 0.8f, bind(&Ninja::EndHit, this));
}

void Ninja::Trace()
{
	if (!collider->isActive) return;

	if (!target) return;

	Vector3 dir = target->position - ninja->position;

	Vector3 cross = Vector3::Cross(dir, ninja->Forward());

	if (cross.y > 0.01f)
		ninja->rotation.y += DELTA;
	else if (cross.y < -0.01f)
		ninja->rotation.y -= DELTA;

	ninja->position -= ninja->Forward() * DELTA;

	SetClip(RUN);
}

void Ninja::EndHit()
{
	SetClip(IDLE);
	collider->isActive = true;
}

void Ninja::EndDie()
{
	ninja->isActive = false;
}

void Ninja::SetHpBar()
{
	float distance = Distance(CAM->position, ninja->position);
	hpBar->scale.x = 10 / distance;
	hpBar->scale.y = 10 / distance;

	Vector3 barPos = ninja->position + Vector3(0, 5, 0);

	hpBar->position = CAM->WorldToScreenPoint(barPos);

	lerpHp = LERP(lerpHp, hp, lerpSpeed * DELTA);
	hpBar->SetLerpValue(lerpHp);
	hpBar->Update();
}

void Ninja::SetLeftHand()
{
	leftHand = instancing->GetTransformByNode(instanceID, 11) * ninja->GetWorld();
}

void Ninja::SetMotions()
{
	//ReadClip("Idle");
	//ReadClip("Run");
	//ReadClip("Attack");
	////ReadClip("Hit"); // 1. BreakPoint here, Start Debugging, go to ReadClip()
	//ReadClip("Hit", 0, true);
	//ReadClip("Dying");
	//
	//clips[HIT]->SetEvent(0.8f, bind(&Ninja::EndHit, this));
	//clips[DYING]->SetEvent(0.9f, bind(&Ninja::EndDie, this));
}

void Ninja::SetClip(AnimState state)
{
	if (this->state != state)
	{
		this->state = state;
		instancing->PlayClip(instanceID, state);
	}
}
