#pragma once

class Transform
{
public:
	string tag;

	bool isActive;

	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

protected:
	Vector3 globalPosition;
	Vector3 globalRotation;
	Vector3 globalScale;

	Vector3 pivot;

	Matrix world;
	Matrix* parent;

public:
	Transform();

	void UpdateWorld();

	void GUIRender();

	Vector3 Right() { return XMVector3Normalize(XMVector3TransformNormal(Vector3(1, 0, 0), world)); }
	Vector3 Up() { return XMVector3Normalize(XMVector3TransformNormal(Vector3(0, 1, 0), world)); }
	Vector3 Forward() { return XMVector3Normalize(XMVector3TransformNormal(Vector3(0, 0, 1), world)); }

	Vector3 GlobalPos() { return globalPosition; }
	Vector3 GlobalRot() { return globalRotation; }
	Vector3 GlobalScale() { return globalScale; }

	Matrix& GetWorld() { return world; }

	void SetParent(Matrix* parent) { this->parent = parent; }
	void SetParent(Transform* transform) { parent = &transform->GetWorld(); }

	void SetPivot(float x, float y, float z) { pivot = { x,y,z }; }

	void Save();
	void Load();
};