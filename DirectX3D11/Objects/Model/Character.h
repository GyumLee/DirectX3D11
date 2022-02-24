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
	AStar* aStar;

public:
	Character();
	~Character();

	void Update();
	void Render();

	void SetTerrain(Terrain* terrain) { this->terrain = terrain; }
	void SetAStar(AStar* aStar) { this->aStar = aStar; }

private:
	void Control();
	void Move();
	void Rotate();
	void Jump();

	void SetClip(AnimState state);

	void SetPath();
};