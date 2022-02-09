#pragma once

class Hinata : public ModelAnimator
{
private:
	enum AnimState
	{
		IDLE, RUN, FIRE
	}state;

	Model* rifle;

	Matrix rightHand;

	float moveSpeed = 5.0f;
	float rotSpeed = 5.0f;

	bool isFire = false;

	Terrain* terrain = nullptr;

	bool isStickCursor = false;
	bool isSkipMouseDelta = false;
	bool isCursorSwitch = false;

	RenderTransform* firePos;

public:
	Hinata();
	~Hinata();

	void Update();
	void Render();
	void GUIRender();

	void SetTerrain(Terrain* terrain) { this->terrain = terrain; }

private:
	void Control();
	void Move();

	void Fire();
	void EndFire();

	void SetClip(AnimState state);

	void SetMotions();
};