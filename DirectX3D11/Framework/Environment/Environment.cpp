#include "Framework.h"

Environment::Environment()
{
	CreateViewport();
	CreateProjection();

	uiViewBuffer = new ViewBuffer();

	samplerState = new SamplerState();
	samplerState->SetState();

	mainCamera = new Camera();

	lightBuffer = new LightBuffer();

	blendState[0] = new BlendState();
	blendState[1] = new BlendState();
	blendState[1]->Alpha(true);

	depthState[0] = new DepthStencilState();
	depthState[1] = new DepthStencilState();
	depthState[1]->DepthEnable(false);
}

Environment::~Environment()
{
	delete mainCamera;

	delete projectionBuffer;
	delete uiViewBuffer;
	delete orthoBuffer;

	delete lightBuffer;

	delete samplerState;

	delete blendState[0];
	delete blendState[1];

	delete depthState[0];
	delete depthState[1];
}

void Environment::SetRender()
{
	DC->RSSetViewports(1, &viewport);

	mainCamera->SetBuffer();
	projectionBuffer->SetVSBuffer(2);

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

void Environment::Render()
{
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
		ImGui::ColorEdit3("COLOR", (float*)&lightBuffer->data.color, ImGuiColorEditFlags_PickerHueWheel);
		ImGui::DragFloat3("DIRECTION", (float*)&lightBuffer->data.direction, 0.01f, -1.0f, 1.0f);
		ImGui::ColorEdit3("AMBIENT LIGHT", (float*)&lightBuffer->data.ambientColor, ImGuiColorEditFlags_PickerHueWheel);
		ImGui::ColorEdit3("AMBIENT CEIL", (float*)&lightBuffer->data.ambientCeil, ImGuiColorEditFlags_PickerHueWheel);

		ImGui::TreePop();
	}

	ImGui::Spacing(); ImGui::Spacing();
}

void Environment::CreateViewport()
{
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = WIN_WIDTH;
	viewport.Height = WIN_HEIGHT;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	DC->RSSetViewports(1, &viewport);
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
