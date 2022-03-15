#pragma once

class ParticleSystem
{
private:
	const UINT MAX_COUNT = 1000;

	struct ParticleData
	{
		bool isLoop = true;
		bool isAdditive = true;
		bool isBillboard = true;
		float duration = 1.0f;
		UINT count = 500;
		Vector3 minVelocity = { -1, -1, -1 };
		Vector3 maxVelocity = { +1, +1, +1 };
		Vector3 minAccelation;
		Vector3 maxAccelation;
		Vector3 minScale = { 1, 1, 1 };
		Vector3 maxScale = { 1, 1, 1 };
		float minStartTime = 0.0f;
		float maxStartTime = 0.0f;
		float minSpeed = 1.0f;
		float maxSpeed = 1.0f;
		float minAngularVelocity = 0.0f;
		float maxAngularVelocity = 0.0f;
		float worldRotationY;
		Float4 startColor = { 1, 1, 1, 1 };
		Float4 endColor = { 1, 1, 1, 1 };
		Float2 tiling = { 1, 1 };
		float spriteSpeed = 1.0f;
	}particleData;

	struct QuadData
	{
		Transform transform;
		Vector3 velocity;
		Vector3 accelation;
		float startTime = 0.0f;
		float speed = 1.0f;
		float angularVelocity = 0.0f;
	};

	Quad* quad;

	vector<Matrix> transforms;
	VertexBuffer* instanceBuffer;
	SpriteBuffer* spriteBuffer;

	vector<QuadData> quads;

	float time = 0.0f;
	float spriteTime = 0.0f;
	UINT drawCount = 0;

	BlendState* blendState[2];
	DepthStencilState* depthState[2];

public:
	ParticleSystem();
	~ParticleSystem();

	void Update();
	void Render();
	void GUIRender();

	void Play(Vector3 pos, Vector3 rot = Vector3());
	void Stop();

	void Save(string file);
	void Load(string file);

private:
	void UpdatePhysical();
	void UpdateColor();
	void UpdateSprite();

	void Init();
};