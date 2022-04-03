#pragma once

class Sword : public Model
{
private:
	BoxCollider* collider;

	class Trail* trail;

	Transform start;
	Transform end;

	Vector3 startPos;
	Vector3 endPos;

public:
	Sword();
	~Sword();

	void Update();
	void Render();
	void GUIRender();

	BoxCollider* GetCollider() { return collider; }
	class Trail* GetTrail() { return trail; }
};