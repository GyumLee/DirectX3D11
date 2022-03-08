#pragma once

class Ninja
{
private:
	enum AnimState
	{
		IDLE, RUN, ATTACK, HIT, DYING
	}state;

	UINT instanceID;

	Matrix leftHand;

	Terrain* terrain = nullptr;

	Collider* collider;

	ProgressBar* hpBar;

	float hp = 100.0f;
	float lerpHp = 100.0f;
	float lerpSpeed = 1.0f;

	Transform* target = nullptr;
	Transform* ninja = nullptr;
	Transform* kunai = nullptr;

	ModelAnimatorInstancing* instancing;

public:
	Ninja(UINT instanceID);
	~Ninja();

	void Update();
	void Render();
	void PostRender();
	void GUIRender();

	void Hit();

	void SetEvent();

	void SetTerrain(Terrain* terrain) { this->terrain = terrain; }
	void SetTarget(Transform* transform) { target = transform; }
	void SetTransform(Transform* transform)
	{
		ninja = transform;
		collider->SetParent(transform);
	}
	void SetKunai(Transform* transform)
	{
		kunai = transform;
		kunai->tag = "Kunai";
		kunai->SetParent(&leftHand);
		kunai->Load();
	}
	void SetInstancing(ModelAnimatorInstancing* instancing) { this->instancing = instancing; }

	Collider* GetCollider() { return collider; }

	bool IsActive() { return ninja->isActive; }
	Transform* GetTransform() { return ninja; }

private:
	void Move();
	void Trace();

	void EndHit();
	void EndDie();

	void SetHpBar();
	void SetLeftHand();
	void SetMotions();

	void SetClip(AnimState state);
};