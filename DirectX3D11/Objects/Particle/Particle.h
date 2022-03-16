#pragma once

class Particle
{
protected:
	const UINT MAX_COUNT = 1000;

	UINT particleCount = 0;

	bool isActive = false;
	Vector3 position;

	Material* material;
	VertexBuffer* vertexBuffer;

	GeometryShader* geometryShader;

	BlendState* blendState[2];
	DepthStencilState* depthState[2];

public:
	Particle();
	~Particle();

	virtual void Update() = 0;
	virtual void Render();
	virtual void GUIRender() = 0;

	virtual void Play(Vector3 position);

	void Stop();
};