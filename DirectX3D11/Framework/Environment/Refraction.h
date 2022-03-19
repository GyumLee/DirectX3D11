#pragma once

class Refraction
{
private:
	WaterBuffer* buffer;

	Texture* normalMap;

	RenderTarget* renderTarget;
	DepthStencil* depthStencil;

	class RenderTexture* targetTexture;

public:
	Refraction(string normalFile);
	~Refraction();

	void Update();

	void SetRefraction();
	void SetRender();
	void PostRender();
	void GUIRender();
};