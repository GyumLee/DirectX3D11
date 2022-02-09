#include "Framework.h"

Bullet::Bullet()
{
	model = new Model("Bullet");
	model->SetParent(this);
	model->Load();

	collider = new SphereCollider();
	collider->tag = "BulletCollider";
	collider->SetParent(model);
	collider->Load();
}

Bullet::~Bullet()
{
	delete collider;
	delete model;
}

void Bullet::Update()
{
	if (!isActive) return;

	position += Forward() * speed * DELTA;

	time += DELTA;
	if (time > DURATION)
	{
		isActive = false;
		time = 0.0f;
	}

	UpdateWorld();
	collider->UpdateWorld();
	model->UpdateWorld();
}

void Bullet::Render()
{
	if (!isActive) return;

	model->Render();
	collider->Render();
}

void Bullet::GUIRender()
{
	collider->GUIRender();
	model->GUIRender();
}

void Bullet::Fire(Vector3 pos, Vector3 rot, float speed)
{
	isActive = true;

	time = 0.0f;

	position = pos;
	rotation = rot;
	this->speed = speed;
}
