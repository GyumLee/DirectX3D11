#include "Framework.h"

Hinata::Hinata()
	: ModelAnimator("Hinata"), state(RIFLE_IDLE)
{
	Load();

	SetMotions();

	rifle = new Model("Rifle");
	rifle->SetParent(&rightHand);
	rifle->Load();

	sword = new Sword();
	sword->SetParent(&rightHand);
	sword->isActive = false;
	sword->GetCollider()->isActive = false;

	firePos = new RenderTransform();
	firePos->tag = "FirePos";
	firePos->SetParent(rifle);
	firePos->Load();
}

Hinata::~Hinata()
{
	delete rifle;
	delete firePos;
	delete sword;
}

void Hinata::Update()
{
	rightHand = GetTransformByNode(204) * world;

	Move();
	Control();

	ModelAnimator::Update();
	rifle->UpdateWorld();
	firePos->UpdateWorld();
	sword->Update();
}

void Hinata::Render()
{
	ModelAnimator::Render();
	rifle->Render();
	sword->Render();

	firePos->Render();
}

void Hinata::GUIRender()
{
	Transform::GUIRender();
	rifle->GUIRender();

	firePos->GUIRender();
	sword->GUIRender();
}

void Hinata::Control()
{
	if (isAttack) return;

	if (MOUSE_CLICK(0))
	{
		isAttack = true;

		if (isSwordMode)
			SetClip(SWORD_ATTACK);
		else
			SetClip(RIFLE_ATTACK);
		
	}

	if (KEY_DOWN(VK_TAB))
	{
		if (isSwordMode)
		{
			isSwordMode = false;
			SetClip(SHEATH_SWORD);
		}
		else
		{
			isSwordMode = true;
			SetClip(PUTBACK_RIFLE);
		}
	}
}

void Hinata::Move()
{
	if (isAttack) return;

	if (KEY_PRESS('W'))
	{
		position -= Forward() * moveSpeed * DELTA;
		SetClip(RIFLE_RUN);
	}
	else if (KEY_PRESS('S'))
	{
		position += Forward() * moveSpeed * DELTA;
		SetClip(RIFLE_RUN);
	}

	if (!ETC::Get()->IsSkipMouseDelta())
	{
		ImVec2 delta = ImGui::GetIO().MouseDelta;
		rotation.y += delta.x * DELTA * rotSpeed;
		CAM->GetRotOffset().x += delta.y * DELTA * rotSpeed;
	}

	float degreeX;
	degreeX = XMConvertToDegrees(CAM->GetRotOffset().x);
	if (degreeX > 0.0f)
		CAM->GetRotOffset().x = XMConvertToRadians(0.0f);
	else if (degreeX < -30.0f)
		CAM->GetRotOffset().x = XMConvertToRadians(-30.0f);

	/*if (KEY_PRESS('A'))
		rotation.y -= rotSpeed * DELTA;
	else if (KEY_PRESS('D'))
		rotation.y += rotSpeed * DELTA;*/

	if (KEY_UP('W') || KEY_UP('S'))
		SetClip(RIFLE_IDLE);

	if (terrain != nullptr)
		position.y = terrain->GetHeight(position);
}

void Hinata::Fire()
{
	Ray ray = CAM->ScreenPointToRay(Vector3(CENTER_X, CENTER_Y, 0.0f));

	Vector3 hitPoint;
	Vector3 destRot;

	Transform* monster = MonsterManager::Get()->RayCollision(ray, &hitPoint);

	if (monster)
	{
		Vector3 direction = (hitPoint - firePos->GlobalPos()).Normalize();
		destRot.y = atan2(direction.x, direction.z);
		destRot.x = asin(-direction.y);
	}
	else
	{
		destRot = CAM->rotation;
	}

	BulletManager::Get()->Fire(firePos->GlobalPos(), destRot, 30.0f);

	/*Vector3 rot = rotation;
	rot.y += XM_PI;
	BulletManager::Get()->Fire(firePos->GlobalPos(), rot);*/
}

void Hinata::EndFire()
{
	isAttack = false;
	SetClip(RIFLE_IDLE);
}

void Hinata::EndSlash()
{
	isAttack = false;
	SetClip(SWORD_IDLE);
}

void Hinata::EndSheath()
{
	SetClip(GRAB_RIFLE);
	rifle->isActive = true;
	sword->isActive = false;
}

void Hinata::EndPutBack()
{
	SetClip(DRAW_SWORD);
	rifle->isActive = false;
	sword->isActive = true;
}

void Hinata::ActiveSword()
{
	sword->GetCollider()->isActive = true;
}

void Hinata::InactiveSword()
{
	sword->GetCollider()->isActive = false;
}

void Hinata::SetIdle()
{
	if (isSwordMode)
		SetClip(SWORD_IDLE);
	else
		SetClip(RIFLE_IDLE);
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
	ReadClip("SwordIdle");
	ReadClip("SwordRun");
	ReadClip("SwordSlash");
	ReadClip("PutBackRifle");
	ReadClip("GrabRifle");
	ReadClip("DrawSword");
	ReadClip("SheathSword");

	clips[RIFLE_ATTACK]->SetEvent(0.5f, bind(&Hinata::Fire, this));
	clips[RIFLE_ATTACK]->SetEvent(0.9f, bind(&Hinata::EndFire, this));
	clips[SHEATH_SWORD]->SetEvent(0.9f, bind(&Hinata::EndSheath, this));
	clips[PUTBACK_RIFLE]->SetEvent(0.9f, bind(&Hinata::EndPutBack, this));
	clips[GRAB_RIFLE]->SetEvent(0.7f, bind(&Hinata::SetIdle, this));
	clips[DRAW_SWORD]->SetEvent(0.7f, bind(&Hinata::SetIdle, this));
	clips[SWORD_ATTACK]->SetEvent(0.5f, bind(&Hinata::ActiveSword, this));
	clips[SWORD_ATTACK]->SetEvent(0.7f, bind(&Hinata::InactiveSword, this));
	clips[SWORD_ATTACK]->SetEvent(0.9f, bind(&Hinata::EndSlash, this));
}
