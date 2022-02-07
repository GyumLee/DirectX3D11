#pragma once

class Camera : public Transform
{
private:
	float moveSpeed;
	float rotSpeed;
	float wheelSpeed;

	ViewBuffer* viewBuffer;

	Matrix view;

	Transform* target;

	float distance;
	float height;

	float rotY;

	float destRot;
	Vector3 destPos;
	Vector3 focusOffset;

	Matrix rotMatrix;

	float moveDamping;
	float rotDamping;

public:
	Camera();
	~Camera();

	void Update();
	void GUIRender();

	void SetBuffer();

	Ray ScreenPointToRay(Vector3 screenPos);

	void SetTarget(Transform* transform) { target = transform; }

	Vector3& GetFocusOffset() { return focusOffset; }

private:
	void FreeMode();
	void FollowMode();

	void SetView();

	void Save();
	void Load();

	void SaveTargetMode();
	void LoadTargetMode();
};