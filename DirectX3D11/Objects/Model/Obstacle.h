#pragma once

class Obstacle : public Model
{
private:
	Collider* collider;

public:
	Obstacle(string name, Collider::Type type);
	~Obstacle();

	void Update();
	void Render();
	void GUIRender();

	Collider* GetCollider() { return collider; }
};