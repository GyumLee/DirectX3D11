#include "Framework.h"

Reflection::Reflection(Transform* transform)
	: target(transform)
{
	renderTarget = new RenderTarget(2048, 2048);
	depthStencil = new DepthStencil(2048, 2048);

	camera = new Camera();

	targetTexture = new RenderTexture(300, 300);
	targetTexture->position = { 150, 150, 0 };
	targetTexture->SetSRV(renderTarget->GetSRV());
	targetTexture->UpdateWorld();
}

Reflection::~Reflection()
{
	delete renderTarget;
	delete depthStencil;
	delete camera;
	delete targetTexture;
}

void Reflection::Update()
{
	camera->position = CAM->position;
	camera->rotation = CAM->rotation;

	camera->rotation.x *= -1.0f;
	camera->position.y = target->position.y * 2.0f - camera->position.y;

	camera->Update();
}

void Reflection::SetReflection()
{
	renderTarget->Set(depthStencil, Float4(1, 1, 1, 1));
	camera->SetBuffer();
}

void Reflection::SetRender()
{
	DC->PSSetShaderResources(10, 1, &renderTarget->GetSRV());
	camera->SetBuffer(10);
}

void Reflection::PostRender()
{
	targetTexture->Render();
}
