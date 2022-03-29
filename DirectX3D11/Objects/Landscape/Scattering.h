#pragma once

class Scattering
{
private:
	ScatteringBuffer* buffer;

	RenderTarget* targets[2];
	DepthStencil* depthStencil;

	RenderTexture* rayleighTexture;
	RenderTexture* mieTexture;

	Quad* quad;

	//Sphere
	float radius;
	UINT slices, stacks;
	UINT vertexCount, indexCount;
	Mesh* mesh;
	Material* material;
	DepthStencilState* depthStencilState[2];

public:
	Scattering();
	~Scattering();

	void PreRender();
	void Render();
	void PostRender();
	void GUIRender();

private:
	void CreateSphere();
};