#include "Framework.h"

Camera::Camera()
	: moveSpeed(200.0f), rotSpeed(0.004f), wheelSpeed(1000.0f),
	target(nullptr), distance(20.0f), height(20.0f),
	moveDamping(5.0f), rotDamping(0.0f)
{
	tag = "CameraTransform";
	viewBuffer = new ViewBuffer();
	view = XMMatrixIdentity();

	Load();
}

Camera::~Camera()
{
	Save();

	delete viewBuffer;
}

void Camera::Update()
{
	if (target)
	{
		FollowMode();
	}
	else
	{
		FreeMode();
	}
}

void Camera::GUIRender()
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
	if (ImGui::TreeNodeEx("CAMERA SETTING", flags, "CAMERA SETTING"))
	{
		ImGui::DragFloat3("POSITION", (float*)&position);
		Vector3 degree = rotation * 180.0f / XM_PI;
		ImGui::DragFloat3("ROTATION", (float*)&degree, 0.1f);
		rotation = degree * XM_PI / 180.0f;
		ImGui::DragFloat("MOVE SPEED", &moveSpeed, 1.0f, 0, 1000.0f);
		ImGui::SliderFloat("ROTATION SPEED", &rotSpeed, 0.0f, 0.02f);
		ImGui::DragFloat("WHEEL SPEED", &wheelSpeed, 1.0f, 0, 2000.0f);

		if (target)
		{
			ImGui::Text("FollowOption");
			ImGui::DragFloat("Distance", &distance, 0.1f);
			ImGui::DragFloat("Height", &height, 0.1f);
			ImGui::DragFloat3("FocusOffset", (float*)&focusOffset, 0.1f);
			Vector3 degree;
			degree.x = XMConvertToDegrees(rotOffset.x);
			degree.y = XMConvertToDegrees(rotOffset.y);
			ImGui::DragFloat3("RotationOffset", (float*)&degree, 1.0f, -180, 180);
			rotOffset.x = XMConvertToRadians(degree.x);
			rotOffset.y = XMConvertToRadians(degree.y);
			ImGui::DragFloat("MoveDamping", &moveDamping, 0.1f, 0.0f, 100.0f);
			ImGui::DragFloat("RotDamping", &rotDamping, 0.1f, 0.0f, 100.0f);

			if (ImGui::Button("Save"))
				SaveTargetMode();
			ImGui::SameLine();
			if (ImGui::Button("Load"))
				LoadTargetMode();
		}

		ImGui::TreePop();
	}
}

void Camera::SetBuffer()
{
	viewBuffer->SetVSBuffer(1);
	viewBuffer->SetGSBuffer(1);
}

Ray Camera::ScreenPointToRay(Vector3 screenPos)
{
	Float2 screenSize(WIN_WIDTH, WIN_HEIGHT);

	Float2 point;
	point.x = (2.0f * (screenPos.x / screenSize.x)) - 1.0f;
	point.y = ((2.0f * (screenPos.y / screenSize.y)) - 1.0f) * -1.0f;

	Matrix projection = Environment::Get()->GetProjection();

	Float4x4 temp;
	XMStoreFloat4x4(&temp, projection);

	point.x /= temp._11;
	point.y /= temp._22;

	Ray ray;
	ray.position = position;

	Vector3 tempPos(point.x, point.y, 1.0f);

	Matrix invView = XMMatrixInverse(nullptr, view);//world
	ray.direction = XMVector3TransformNormal(tempPos, invView);
	ray.direction = ray.direction.Normalize();

	return ray;
}

Vector3 Camera::WorldToScreenPoint(Vector3 worldPos)
{
	Vector3 screenPos;

	screenPos = XMVector3TransformCoord(worldPos, view);
	screenPos = XMVector3TransformCoord(screenPos, Environment::Get()->GetProjection());
	//NDC -> -1~1 -> 0~width

	screenPos = (screenPos + 1.0f) * 0.5f;//-1~1 -> 0~1

	screenPos.x *= WIN_WIDTH;
	screenPos.y *= WIN_HEIGHT;

	return screenPos;
}

void Camera::SetTarget(Transform* transform)
{
	target = transform;
	target->UpdateWorld();

	destRot.y = target->rotation.y + XM_PI;
	rotMatrix = XMMatrixRotationY(destRot.y + rotOffset.y);

	Vector3 forward = XMVector3TransformNormal(Vector3(0, 0, 1), rotMatrix);

	position = forward * -distance + target->GlobalPos();
	position.y += height;
}

void Camera::FreeMode()
{
	if (!ImGui::GetIO().WantCaptureMouse)
	{
		if (MOUSE_PRESS(1))
		{
			float speed = moveSpeed;
			if (KEY_PRESS(VK_SHIFT)) speed *= 2.0f;
			if (KEY_PRESS(VK_CONTROL)) speed *= 0.5f;

			if (KEY_PRESS('W')) position += Forward() * speed * DELTA;
			if (KEY_PRESS('S')) position -= Forward() * speed * DELTA;
			if (KEY_PRESS('D')) position += Right() * speed * DELTA;
			if (KEY_PRESS('A')) position -= Right() * speed * DELTA;
			if (KEY_PRESS('E')) position += Up() * speed * DELTA;
			if (KEY_PRESS('Q')) position -= Up() * speed * DELTA;

			ImVec2 delta = ImGui::GetIO().MouseDelta;
			rotation.x += delta.y * rotSpeed;
			rotation.y += delta.x * rotSpeed;
		}

		position += Forward() * ImGui::GetIO().MouseWheel * wheelSpeed * DELTA;
	}

	SetView();
}

void Camera::FollowMode()
{
	destRot.y = LERP(destRot.y, target->rotation.y + XM_PI, rotDamping * DELTA);

	rotMatrix = XMMatrixRotationY(destRot.y + rotOffset.y);

	Vector3 forward = XMVector3TransformNormal(Vector3(0, 0, 1), rotMatrix);

	destPos = target->GlobalPos() - forward * distance;
	destPos.y += height;

	position = LERP(position, destPos, moveDamping * DELTA);

	Vector3 offset = XMVector3TransformCoord(focusOffset, rotMatrix);
	Vector3 focus = target->GlobalPos() + offset;

	Vector3 direction = (focus - position).Normalize();

	destRot.x = asin(-direction.y) + rotOffset.x;

	rotation.x = destRot.x;
	rotation.y = atan2(direction.x, direction.z);

	SetView();
}

void Camera::SetView()
{
	UpdateWorld();
	view = XMMatrixInverse(nullptr, world);

	viewBuffer->Set(view, world);
}

void Camera::Save()
{
	BinaryWriter w("TextData/Environment/Camera.info");

	w.Vector(position);
	w.Vector(rotation);
	w.Float(moveSpeed);
	w.Float(rotSpeed);
}

void Camera::Load()
{
	BinaryReader r("TextData/Environment/Camera.info");

	if (r.IsFailed()) return;

	position = r.Vector();
	rotation = r.Vector();
	moveSpeed = r.Float();
	rotSpeed = r.Float();
}

void Camera::SaveTargetMode()
{
	BinaryWriter w("TextData/Environment/CameraTarget.info");

	w.Float(distance);
	w.Float(height);
	w.Vector(rotOffset);
	w.Vector(focusOffset);
	w.Float(moveDamping);
	w.Float(rotDamping);
}

void Camera::LoadTargetMode()
{
	BinaryReader r("TextData/Environment/CameraTarget.info");

	if (r.IsFailed()) return;

	distance = r.Float();
	height = r.Float();
	rotOffset = r.Vector();
	focusOffset = r.Vector();
	moveDamping = r.Float();
	rotDamping = r.Float();
}
