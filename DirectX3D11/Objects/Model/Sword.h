#pragma once

class Sword : public Model
{
private:
	BoxCollider* collider;

public:
	Sword();
	~Sword();

	void Update();
	void Render();
	void GUIRender();

	BoxCollider* GetCollider() { return collider; }
};