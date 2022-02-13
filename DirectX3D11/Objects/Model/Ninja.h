#pragma once

class Ninja : public ModelAnimator
{
private:
	enum AnimState
	{
		IDLE, RUN, ATTACK, HIT, DYING
	}state;

	Model* kunai;

	Matrix rightHand;

	Terrain* terrain = nullptr;

	Collider* collider;

	ProgressBar* hpBar;

	float hp = 100.0f;
	float lerpHp = 100.0f;
	float lerpSpeed = 1.0f;

	Transform* target = nullptr;

public:
	Ninja();
	~Ninja();

	void Update();
	void Render();
	void PostRender();
	void GUIRender();

	void SetTerrain(Terrain* terrain) { this->terrain = terrain; }
	void SetTarget(Transform* transform) { target = transform; }

	Collider* GetCollider() { return collider; }

private:
	void Move();
	void Hit();
	void Trace();

	void EndHit();
	void EndDie();

	void SetHpBar();
	void SetRightHand();
	void SetMotions();

	void SetClip(AnimState state);
};