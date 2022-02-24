#pragma once

class Character : public ModelAnimator
{
private:
	enum AnimState
	{
		IDLE, RUN, JUMP
	}state;

	float moveSpeed;
	float rotSpeed;
	float gravity;
	float jumpPower;

	Vector3 velocity;
	
	Vector3 destPos;
	vector<Vector3> path;

	Terrain* terrain;

public:
	Character();
	~Character();

	void Update();
	void Render();

	void SetTerrain(Terrain* terrain) { this->terrain = terrain; }

private:
	void Control();
	void Move();
	void Rotate();
	void Jump();

	void SetClip(AnimState state);

	void SetPath();
};