#pragma once

class Reflection
{
private:
	Transform* target;

	RenderTarget* renderTarget;
	DepthStencil* depthStencil;

	Camera* camera;

	class RenderTexture* targetTexture;

public:
	Reflection(Transform* transform);
	~Reflection();

	void Update();

	void SetReflection();
	void SetRender();
	void PostRender();
};