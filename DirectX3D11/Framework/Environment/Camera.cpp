#include "Framework.h"

Camera::Camera()
	: moveSpeed(200.0f), rotSpeed(0.004f), wheelSpeed(1000.0f)
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
	FreeMode();
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

		ImGui::TreePop();
	}
}

void Camera::SetBuffer()
{
	viewBuffer->SetVSBuffer(1);
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

void Camera::FreeMode()
{
	if (!ImGui::GetIO().WantCaptureMouse)
	{
		if (MOUSE_PRESS(1))
		{
			if (KEY_PRESS('W')) position += Forward() * moveSpeed * DELTA;
			if (KEY_PRESS('S')) position -= Forward() * moveSpeed * DELTA;
			if (KEY_PRESS('D')) position += Right() * moveSpeed * DELTA;
			if (KEY_PRESS('A')) position -= Right() * moveSpeed * DELTA;
			if (KEY_PRESS('E')) position += Up() * moveSpeed * DELTA;
			if (KEY_PRESS('Q')) position -= Up() * moveSpeed * DELTA;

			ImVec2 delta = ImGui::GetIO().MouseDelta;
			rotation.x += delta.y * rotSpeed;
			rotation.y += delta.x * rotSpeed;
		}

		position += Forward() * ImGui::GetIO().MouseWheel * wheelSpeed * DELTA;
	}

	UpdateWorld();
	SetView();
}

void Camera::SetView()
{
	//Vector3 focus = position + Forward();
	//view = XMMatrixLookAtLH(position, focus, Up());

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
