#include "Framework.h"

Shadow::Shadow(UINT width, UINT height)
	: width(width), height(height)
{
	renderTarget = new RenderTarget(width, height);
	depthStencil = new DepthStencil(width, height);

	depthTexture = new RenderTexture(300, 300);
	depthTexture->position = { 150, 150, 0 };
	depthTexture->SetSRV(renderTarget->GetSRV());
	depthTexture->UpdateWorld();

	viewBuffer = new ViewBuffer();
	projectionBuffer = new ProjectionBuffer();
}

Shadow::~Shadow()
{
	delete renderTarget;
	delete depthStencil;
	delete depthTexture;
	delete viewBuffer;
	delete projectionBuffer;
}

void Shadow::SetDepthRender()
{
	renderTarget->Set(depthStencil, Float4(1, 1, 1, 1));
	SetViewProjection();
}

void Shadow::SetRender()
{
	viewBuffer->SetVSBuffer(11);
	projectionBuffer->SetVSBuffer(12);

	DC->PSSetShaderResources(10, 1, &renderTarget->GetSRV());
}

void Shadow::PostRender()
{
	depthTexture->Render();
}

void Shadow::GUIRender()
{
	ImGui::SliderFloat("Scale", &scale, 1.0f, 10.0f);
}

void Shadow::SetViewProjection()
{
	Vector3 lightPos = Environment::Get()->GetLight(0)->position;

	Matrix view = XMMatrixLookAtLH(lightPos, Vector3(), Vector3(0, 1, 0));
	//Matrix projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, 1.0f, 0.1f, 1000.0f);
	float length = lightPos.Length() * scale;
	Matrix projection = XMMatrixOrthographicLH(length, length, 0.1f, 1000.0f);

	viewBuffer->Set(view, view);
	projectionBuffer->Set(projection);

	viewBuffer->SetVSBuffer(1);
	projectionBuffer->SetVSBuffer(2);
}
