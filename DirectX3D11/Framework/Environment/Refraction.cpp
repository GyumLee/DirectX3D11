#include "Framework.h"

Refraction::Refraction(string normalFile)
{
	buffer = new WaterBuffer();

	normalMap = Texture::Add(normalFile);

	renderTarget = new RenderTarget(2048, 2048);
	depthStencil = new DepthStencil(2048, 2048);

	targetTexture = new RenderTexture(300, 300);
	targetTexture->position = { 450, 150, 0 };
	targetTexture->SetSRV(renderTarget->GetSRV());
	targetTexture->UpdateWorld();
}

Refraction::~Refraction()
{
	delete renderTarget;
	delete depthStencil;
	delete buffer;
	delete targetTexture;
}

void Refraction::Update()
{
	buffer->data.waveTime += DELTA;
}

void Refraction::SetRefraction()
{
	renderTarget->Set(depthStencil);
}

void Refraction::SetRender()
{
	buffer->SetPSBuffer(10);
	normalMap->Set(2);
	DC->PSSetShaderResources(11, 1, &renderTarget->GetSRV());
}

void Refraction::PostRender()
{
	targetTexture->Render();
}

void Refraction::GUIRender()
{
	ImGui::Text("Water Option");
	ImGui::ColorEdit4("WaveColor", (float*)&buffer->data.color);

	ImGui::SliderFloat("WaveSpeed", &buffer->data.waveSpeed, 0.0f, 1.0f);
	ImGui::SliderFloat("WaveScale", &buffer->data.waveScale, 0.0f, 1.0f);
	ImGui::SliderFloat("WaveShininess", &buffer->data.waveShininess, 1.0f, 50.0f);
	ImGui::SliderFloat("WaveFresnel", &buffer->data.fresnel, 0.0f, 1.0f);
}
