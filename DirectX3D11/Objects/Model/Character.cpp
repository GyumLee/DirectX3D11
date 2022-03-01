#include "Framework.h"

Character::Character()
	: ModelAnimator("Character"), state(IDLE),
	moveSpeed(10.0f), rotSpeed(5.0f), gravity(9.8f), jumpPower(0.0f)
{
	ReadClip("idle", 1);
	ReadClip("run", 1);
	ReadClip("jump", 1);

	Load();
}

Character::~Character()
{
}

void Character::Update()
{
	Control();
	Move();
	Rotate();
	Jump();

	ModelAnimator::Update();
}

void Character::Render()
{
	ModelAnimator::Render();
}

void Character::Control()
{
	if (MOUSE_CLICK(0) && !ImGui::GetIO().WantCaptureMouse)
	{
		destPos = terrain->Picking();

		Ray ray;
		ray.position = position;
		ray.direction = destPos - position;

		float distance = ray.direction.Length();

		if (aStar->CollisionObstacle(ray, distance))
		{
			SetPath();
		}
		else
		{
			path.clear();
			path.push_back(destPos);
		}
	}

	if (state != JUMP && KEY_DOWN(VK_SPACE))
	{
		SetClip(JUMP);

		jumpPower = 10.0f;
	}
}

void Character::Move()
{
	if (path.empty())
	{
		if (state != JUMP)
			SetClip(IDLE);
		return;
	}

	if (state != JUMP)
		SetClip(RUN);

	Vector3 dest = path.back();

	Vector3 direction = dest - position;
	direction.y = 0.0f;

	if (direction.Length() < 0.1f)
		path.pop_back();

	velocity = direction.Normalize();

	position += velocity * moveSpeed * DELTA;
}

void Character::Rotate()
{
	if (state == IDLE) return;

	Vector3 cross = Vector3::Cross(Forward(), velocity);

	if (cross.y < 0)
		rotation.y += rotSpeed * DELTA;
	else if (cross.y > 0)
		rotation.y -= rotSpeed * DELTA;
}

void Character::Jump()
{
	float height = terrain->GetHeight(position);

	if (state != JUMP)
	{
		position.y = height;
		return;
	}

	jumpPower -= gravity * DELTA;
	position.y += jumpPower * DELTA;

	if (position.y <= height)
	{
		position.y = height;
		jumpPower = 0.0f;
		SetClip(IDLE);
	}
}

void Character::SetClip(AnimState state)
{
	if (this->state != state)
	{
		this->state = state;
		PlayClip(state);
	}
}

void Character::SetPath()
{
	int start = aStar->FindCloseNode(position);
	int end = aStar->FindCloseNode(destPos);

	aStar->GetPath(start, end, path);
	aStar->MakeDirectPath(position, destPos, path);

	path.insert(path.begin(), destPos);

	UINT pathSize = path.size();

	while (path.size() > 2)
	{
		vector<Vector3> tempPath = path;
		tempPath.erase(tempPath.begin());
		tempPath.pop_back();

		Vector3 start = path.back();
		Vector3 end = path.front();

		aStar->MakeDirectPath(start, end, tempPath);

		path.clear();

		path = tempPath;
		path.insert(path.begin(), end);
		path.push_back(start);

		if (pathSize == path.size())
			break;
		else
			pathSize = path.size();
	}
}
