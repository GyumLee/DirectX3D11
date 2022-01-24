#pragma once

class Environment : public Singleton<Environment>
{
private:
	friend class Singleton;

	Camera* mainCamera;
	Grid* grid;

	ProjectionBuffer* projectionBuffer;
	Matrix projection;

	LightBuffer* lightBuffer;

	D3D11_VIEWPORT viewport;

	SamplerState* samplerState;

	Environment();
	~Environment();

public:
	void SetRender();
	void Render();
	void GUIRender();

	Camera* GetMainCamera() { return mainCamera; }
	Matrix GetProjection() { return projection; }

private:
	void CreateViewport();
	void CreateProjection();
};