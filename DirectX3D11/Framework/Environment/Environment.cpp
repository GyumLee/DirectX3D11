#include "Framework.h"

Environment::Environment()
{
	SetViewport();
	CreateProjection();

	uiViewBuffer = new ViewBuffer();	

	samplerState = new SamplerState();
	samplerState->SetState();

	mainCamera = new Camera();
	mainCamera->Load();
	frustum = new Frustum(projection);

	lightBuffer = new LightBuffer();

	blendState[0] = new BlendState();
	blendState[1] = new BlendState();
	blendState[1]->Alpha(true);

	depthState[0] = new DepthStencilState();
	depthState[1] = new DepthStencilState();
	depthState[1]->DepthEnable(false);

	for (UINT i = 0; i < MAX_LIGHT; i++)
		lightTransforms[i] = new RenderTransform();	
}

Environment::~Environment()
{
	mainCamera->Save();
	delete mainCamera;
	delete frustum;

	delete projectionBuffer;
	delete uiViewBuffer;
	delete orthoBuffer;

	delete lightBuffer;

	delete samplerState;

	delete blendState[0];
	delete blendState[1];

	delete depthState[0];
	delete depthState[1];

	for (RenderTransform* lightTransform : lightTransforms)
		delete lightTransform;
}

void Environment::SetRender()
{
	DC->RSSetViewports(1, &viewport);

	mainCamera->SetBuffer();
	projectionBuffer->SetVSBuffer(2);
	projectionBuffer->SetGSBuffer(2);

	lightBuffer->SetPSBuffer(0);

	blendState[0]->SetState();
	depthState[0]->SetState();
}

void Environment::SetPostRender()
{
	uiViewBuffer->SetVSBuffer(1);
	orthoBuffer->SetVSBuffer(2);

	blendState[1]->SetState();
	depthState[1]->SetState();
}

void Environment::GUIRender()
{
	ImGui::TextColored(ImVec4(0, 0.8f, 0.8f, 1), "Environment Component");
	ImGui::Spacing();

	mainCamera->GUIRender();
	ImGui::Spacing();

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
	if(ImGui::TreeNodeEx("LIGHT SETTING", flags, "LIGHT SETTING"))
	{
		if (ImGui::Button("Add"))
			lightBuffer->data.lightCount++;
		ImGui::SameLine();
		if (ImGui::Button("Remove"))
			lightBuffer->data.lightCount--;

		for (UINT i = 0; i < lightBuffer->data.lightCount; i++)
		{
			SetLight(i);
		}

		ImGui::ColorEdit3("AMBIENT LIGHT", (float*)&lightBuffer->data.ambientColor, ImGuiColorEditFlags_PickerHueWheel);
		ImGui::ColorEdit3("AMBIENT CEIL", (float*)&lightBuffer->data.ambientCeil, ImGuiColorEditFlags_PickerHueWheel);

		ImGui::TreePop();
	}

	ImGui::Spacing(); ImGui::Spacing();
}

LightBuffer::Light* Environment::AddLight()
{
	int index = lightBuffer->data.lightCount++;

	return &lightBuffer->data.lights[index];
}

void Environment::LightRender()
{
	for (int i = 0; i < lightBuffer->data.lightCount; i++)
	{
		LightBuffer::Light light = lightBuffer->data.lights[i];

		lightTransforms[i]->position = light.position;
		lightTransforms[i]->rotation.y = atan2(light.direction.x, light.direction.z);
		lightTransforms[i]->rotation.x = asin(-light.direction.y);
		if (light.active)
		{
			lightTransforms[i]->UpdateWorld();
			lightTransforms[i]->Render();
		}
	}
}

void Environment::SetViewport(UINT width, UINT height)
{
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	//DC->RSSetViewports(1, &viewport);
}

void Environment::CreateProjection()
{
	projectionBuffer = new ProjectionBuffer();

	projection = XMMatrixPerspectiveFovLH(XM_PIDIV4,
		(float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 1000.0f);

	projectionBuffer->Set(projection);

	orthoBuffer = new ProjectionBuffer();

	Matrix ortho = XMMatrixOrthographicOffCenterLH(0.0f, WIN_WIDTH, 0.0f, WIN_HEIGHT, -1.0f, 1.0f);
	orthoBuffer->Set(ortho);
}

void Environment::SetLight(UINT index)
{
	string name = "Light_" + to_string(index);
	LightBuffer::Light& light = lightBuffer->data.lights[index];

	if (ImGui::TreeNode(name.c_str()))
	{
		ImGui::Checkbox("Active", (bool*)&light.active);

		const char* list[] = { "Directional", "Point", "Spot", "Capsule" };
		ImGui::Combo("Type", (int*)&light.type, list, 4);

		ImGui::ColorEdit3("COLOR", (float*)&light.color, ImGuiColorEditFlags_PickerHueWheel);
		ImGui::DragFloat3("DIRECTION", (float*)&light.direction, 0.01f, -1.0f, 1.0f);

		ImGui::DragFloat3("POSITION", (float*)&light.position);
		ImGui::DragFloat("RANGE", (float*)&light.range);

		ImGui::SliderFloat("INNER", &light.inner, 0, light.outer);
		ImGui::SliderFloat("OUTER", &light.outer, light.inner, 180);
		ImGui::SliderFloat("LENGTH", &light.length, 0, 200);

		ImGui::TreePop();
	}
}
