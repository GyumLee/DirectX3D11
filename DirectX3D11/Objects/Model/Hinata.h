#pragma once

class Hinata : public ModelAnimator
{
private:
	enum AnimState
	{
		RIFLE_IDLE, RIFLE_RUN, RIFLE_ATTACK,
		SWORD_IDLE, SWORD_RUN, SWORD_ATTACK,
		PUTBACK_RIFLE, GRAB_RIFLE,
		DRAW_SWORD, SHEATH_SWORD,
		MAX
	}state;

	Model* rifle;
	Sword* sword;

	Matrix rightHand;

	float moveSpeed = 5.0f;
	float rotSpeed = 5.0f;

	bool isAttack = false;

	Terrain* terrain = nullptr;
	
	RenderTransform* firePos;

	bool isSwordMode;

public:
	Hinata();
	~Hinata();

	void Update();
	void Render();
	void GUIRender();

	void SetTerrain(Terrain* terrain) { this->terrain = terrain; }
	Collider* GetSwordCollider() { return sword->GetCollider(); }

private:
	void Control();
	void Move();

	void Fire();
	void EndFire();
	void EndSlash();

	void EndSheath();
	void EndPutBack();

	void ActiveSword();
	void InactiveSword();

	void SetIdle();

	void SetClip(AnimState state);

	void SetMotions();
};