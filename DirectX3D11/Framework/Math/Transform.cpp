#include "Framework.h"

Transform::Transform()
	: scale(1, 1, 1), parent(nullptr), isActive(true)
{
	world = XMMatrixIdentity();
}

void Transform::UpdateWorld()
{
	world = XMMatrixTransformation(pivot, XMQuaternionIdentity(), scale,
		pivot, XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z),
		position);

	if (parent != nullptr)
		world *= *parent;

	XMMatrixDecompose(&globalScale.data, &globalRotation.data, &globalPosition.data, world);
}

void Transform::GUIRender()
{
	if (ImGui::TreeNode(tag.c_str()))
	{
		ImGui::Checkbox(tag.c_str(), &isActive);
		ImGui::DragFloat3((tag + "_Pos").c_str(), (float*)&position, 0.1f);
		Vector3 degree = rotation * 180.0f / XM_PI;
		ImGui::DragFloat3((tag + "_Rot").c_str(), (float*)&degree, 0.1f);
		rotation = degree * XM_PI / 180.0f;
		ImGui::DragFloat3((tag + "_Scale").c_str(), (float*)&scale, 0.1f);
		//ImGui::DragFloat3((tag + "_Pivot").c_str(), (float*)&pivot, 0.1f);

		if (ImGui::Button((tag + "Save").c_str()))
			Save();
		ImGui::SameLine();
		if (ImGui::Button((tag + "Load").c_str()))
			Load();

		ImGui::TreePop();
	}
}

void Transform::Save()
{
	BinaryWriter w("TextData/Transforms/" + tag + ".srt");

	w.Vector(position);
	w.Vector(rotation);
	w.Vector(scale);
	w.Vector(pivot);
}

void Transform::Load()
{
	BinaryReader r("TextData/Transforms/" + tag + ".srt");

	if (r.IsFailed()) return;

	position = r.Vector();
	rotation = r.Vector();
	scale = r.Vector();
	pivot = r.Vector();
}
