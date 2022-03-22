#pragma once

class Shadow
{
private:
	UINT width, height;

	RenderTarget* renderTarget;
	DepthStencil* depthStencil;

	class RenderTexture* depthTexture;

	ViewBuffer* viewBuffer;
	ProjectionBuffer* projectionBuffer;

	float scale = 1.0f;

public:
	Shadow(UINT width = 2048, UINT height = 2048);
	~Shadow();

	void SetDepthRender();
	void SetRender();
	void PostRender();
	void GUIRender();

private:
	void SetViewProjection();
};