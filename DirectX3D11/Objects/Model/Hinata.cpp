#include "Framework.h"

Hinata::Hinata()
	: ModelAnimator("Hinata"), state(MAX)
{
	Load();

	SetMotions();

	rifle = new Model("Rifle");
	rifle->SetParent(&rightHand);
	rifle->Load();

	firePos = new RenderTransform();
	firePos->tag = "FirePos";
	firePos->SetParent(rifle);
	firePos->Load();
}

Hinata::~Hinata()
{
	delete rifle;
	delete firePos;
}

void Hinata::Update()
{
	rightHand = GetTransformByNode(204) * world;

	Move();
	Control();

	ModelAnimator::Update();
	rifle->UpdateWorld();

	firePos->UpdateWorld();
}

void Hinata::Render()
{
	ModelAnimator::Render();
	rifle->Render();

	firePos->Render();
}

void Hinata::GUIRender()
{
	Transform::GUIRender();
	rifle->GUIRender();

	firePos->GUIRender();
}

void Hinata::Control()
{
	if (isFire) return;

	if (MOUSE_CLICK(0))
	{
		isFire = true;
		SetClip(FIRE);
	}
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
		SetClip(IDLE);

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

	clips[FIRE]->SetEvent(0.5f, bind(&Hinata::Fire, this));
	clips[FIRE]->SetEvent(0.9f, bind(&Hinata::EndFire, this));
}
