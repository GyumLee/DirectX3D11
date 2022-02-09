#pragma once

class Bullet : public Transform
{
private:
	const float DURATION = 20.0f;

	Model* model;

	float speed = 0.1f;
	float time = 0.0f;

	SphereCollider* collider;

public:
	Bullet();
	~Bullet();

	void Update();
	void Render();
	void GUIRender();

	void Fire(Vector3 pos, Vector3 rot, float speed = 10.0f);

	SphereCollider* GetCollider() { return collider; }
};