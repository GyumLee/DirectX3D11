#pragma once

class Environment : public Singleton<Environment>
{
private:
	friend class Singleton;

	Camera* mainCamera;
	Frustum* frustum;

	ProjectionBuffer* projectionBuffer;
	ViewBuffer* uiViewBuffer;
	ProjectionBuffer* orthoBuffer;

	Matrix projection;

	LightBuffer* lightBuffer;

	D3D11_VIEWPORT viewport;

	SamplerState* samplerState;

	BlendState* blendState[2];
	DepthStencilState* depthState[2];

	class RenderTransform* lightTransforms[MAX_LIGHT];

	Environment();
	~Environment();

public:
	void SetRender();
	void SetPostRender();

	void Render();
	void GUIRender();

	LightBuffer::Light* AddLight();

	void LightRender();

	Camera* GetMainCamera() { return mainCamera; }
	Frustum* GetFrustum() { return frustum; }
	Matrix GetProjection() { return projection; }

	LightBuffer::Light* GetLight(UINT index) { return &lightBuffer->data.lights[index]; }

private:
	void CreateViewport();
	void CreateProjection();

	void SetLight(UINT index);
};