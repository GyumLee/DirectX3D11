#pragma once

struct Ray
{
	Vector3 position;
	Vector3 direction;
};

class Camera : public Transform
{
private:
	float moveSpeed;
	float rotSpeed;
	float wheelSpeed;

	ViewBuffer* viewBuffer;

	Matrix view;

public:
	Camera();
	~Camera();

	void Update();
	void GUIRender();

	void SetBuffer();

	Ray ScreenPointToRay(Vector3 screenPos);

private:
	void FreeMode();

	void SetView();

	void Save();
	void Load();
};