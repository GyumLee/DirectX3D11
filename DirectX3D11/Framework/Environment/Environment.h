#pragma once

class Environment : public Singleton<Environment>
{
private:
	friend class Singleton;

	Camera* mainCamera;

	ProjectionBuffer* projectionBuffer;
	ViewBuffer* uiViewBuffer;
	ProjectionBuffer* orthoBuffer;

	Matrix projection;

	LightBuffer* lightBuffer;

	D3D11_VIEWPORT viewport;

	SamplerState* samplerState;

	BlendState* blendState[2];

	Environment();
	~Environment();

public:
	void SetRender();
	void SetPostRender();

	void Render();
	void GUIRender();

	Camera* GetMainCamera() { return mainCamera; }
	Matrix GetProjection() { return projection; }

private:
	void CreateViewport();
	void CreateProjection();
};